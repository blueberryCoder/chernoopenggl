//
// Created by blueberry on 2026/2/10.
//

#include "TestSSAO.h"

#include "../FileUtil.h"
#include "../WindowManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <iostream>
#include <random>
#include <string>

namespace {
    float Lerp(float a, float b, float f) {
        return a + f * (b - a);
    }
}

namespace test {

    TestSSAO::TestSSAO() : m_Rng(1337) {
        SetupCursorCallback();
        m_WasDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
        m_WasBlendEnabled = glIsEnabled(GL_BLEND);
        GLCall(glEnable(GL_DEPTH_TEST));

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Model = std::make_shared<Model>(FileUtil::shared().GetPath("./models/backpack/backpack.obj"));

        m_GeometryShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/ssao_geometry.shader"));
        m_SSAOShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/ssao.shader"));

        m_QuadVBO = std::make_shared<VertexBuffer>(m_QuadVertices, sizeof(m_QuadVertices));
        m_QuadIBO = std::make_shared<IndexBuffer>(m_QuadIndices, 6);
        VertexBufferLayout quadLayout{};
        quadLayout.Push<float>(3);
        quadLayout.Push<float>(2);
        m_QuadVAO = std::make_shared<VertexArray>();
        m_QuadVAO->AddBuffer(*m_QuadVBO, quadLayout);

        int width = 0;
        int height = 0;
        WindowManager::shared().GetFramebufferSize(width, height);
        if (width <= 0 || height <= 0) {
            width = 960;
            height = 540;
        }
        CreateGBuffer(width, height);

        BuildKernel();
        CreateNoiseTexture();

        m_SSAOShader->Bind();
        m_SSAOShader->SetUniform1i("gPosition", 0);
        m_SSAOShader->SetUniform1i("gNormal", 1);
        m_SSAOShader->SetUniform1i("texNoise", 2);
        for (size_t i = 0; i < m_SSAOKernel.size(); ++i) {
            m_SSAOShader->SetUniformVec3f("samples[" + std::to_string(i) + "]", m_SSAOKernel[i]);
        }
        m_SSAOShader->Unbind();

    }

    TestSSAO::~TestSSAO() {
        if (m_NoiseTexture != 0) {
            GLCall(glDeleteTextures(1, &m_NoiseTexture));
            m_NoiseTexture = 0;
        }
        if (!m_WasDepthEnabled) {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
    }

    void TestSSAO::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        int fbWidth = 0;
        int fbHeight = 0;
        WindowManager::shared().GetFramebufferSize(fbWidth, fbHeight);
        if (fbWidth <= 0 || fbHeight <= 0) {
            fbWidth = 960;
            fbHeight = 540;
        }
        if (fbWidth != m_GBufferWidth || fbHeight != m_GBufferHeight) {
            CreateGBuffer(fbWidth, fbHeight);
        }

        float aspect = (fbHeight != 0)
            ? static_cast<float>(fbWidth) / static_cast<float>(fbHeight)
            : (960.0f / 540.0f);

        m_View = m_Camera->GetViewMatrix();
        float fov = glm::radians(m_Camera->GetZoom());
        m_Projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

        m_GeometryShader->Bind();
        m_GeometryShader->SetUniformMat4f("view", m_View);
        m_GeometryShader->SetUniformMat4f("projection", m_Projection);

        m_SSAOShader->Bind();
        m_SSAOShader->SetUniformMat4f("projection", m_Projection);

    }

    void TestSSAO::OnRender() {
        if (m_WasBlendEnabled) {
            GLCall(glDisable(GL_BLEND));
        }

        int fbWidth = m_GBufferWidth;
        int fbHeight = m_GBufferHeight;
        if (fbWidth > 0 && fbHeight > 0) {
            GLCall(glViewport(0, 0, fbWidth, fbHeight));
        }

        m_GBuffer->Bind();
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_GeometryShader->Bind();
        m_GeometryShader->SetUniform1i("invertedNormals", 0);
        glm::mat4 model = glm::mat4(1.0f);
        m_GeometryShader->SetUniformMat4f("model", model);
        m_Model->Draw(*m_GeometryShader);
        m_GBuffer->Unbind();

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);
        if (viewportWidth <= 0 || viewportHeight <= 0) {
            viewportWidth = 960;
            viewportHeight = 540;
        }
        GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glDisable(GL_DEPTH_TEST));

        m_SSAOShader->Bind();
        m_GPosition->Bind(0);
        m_GNormal->Bind(1);
        GLCall(glActiveTexture(GL_TEXTURE2));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_NoiseTexture));
        m_Renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_SSAOShader);

        if (m_WasBlendEnabled) {
            GLCall(glEnable(GL_BLEND));
        }
    }

    void TestSSAO::OnImGuiRender() {
        ImGui::Text("SSAO (geometry + AO)");
    }

    void TestSSAO::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestSSAO::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestSSAO::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }

    void TestSSAO::CreateGBuffer(int width, int height) {
        m_GBufferWidth = width;
        m_GBufferHeight = height;

        TextureInitParams positionParams{};
        positionParams.WRAP_S = GL_CLAMP_TO_EDGE;
        positionParams.WRAP_T = GL_CLAMP_TO_EDGE;
        positionParams.MIN_FILTER = GL_NEAREST;
        positionParams.MAG_FILTER = GL_NEAREST;
        positionParams.width = width;
        positionParams.height = height;
        positionParams.internalFormat = GL_RGB16F;

        TextureInitParams normalParams = positionParams;

        TextureInitParams albedoParams{};
        albedoParams.WRAP_S = GL_CLAMP_TO_EDGE;
        albedoParams.WRAP_T = GL_CLAMP_TO_EDGE;
        albedoParams.MIN_FILTER = GL_NEAREST;
        albedoParams.MAG_FILTER = GL_NEAREST;
        albedoParams.width = width;
        albedoParams.height = height;
        albedoParams.internalFormat = GL_RGB;

        m_GPosition = std::make_shared<Texture>(positionParams);
        m_GNormal = std::make_shared<Texture>(normalParams);
        m_GAlbedo = std::make_shared<Texture>(albedoParams);

        RenderBufferInitParams depthParams{};
        depthParams.width = width;
        depthParams.height = height;
        m_DepthRbo = std::make_shared<RenderBuffer>(depthParams);

        m_GBuffer = std::make_shared<FrameBuffer>();
        m_GBuffer->AttachColor(m_GPosition, 0);
        m_GBuffer->AttachColor(m_GNormal, 1);
        m_GBuffer->AttachColor(m_GAlbedo, 2);
        m_GBuffer->AttachDepthStencil(m_DepthRbo);

        GLenum attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        GLCall(glDrawBuffers(3, attachments));
        GLCall(auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "SSAO GBuffer incomplete, status=" << status << std::endl;
        }
        m_GBuffer->Unbind();
    }

    void TestSSAO::BuildKernel() {
        m_SSAOKernel.clear();
        m_SSAOKernel.reserve(64);
        std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
        for (unsigned int i = 0; i < 64; ++i) {
            glm::vec3 sample(
                randomFloats(m_Rng) * 2.0f - 1.0f,
                randomFloats(m_Rng) * 2.0f - 1.0f,
                randomFloats(m_Rng));
            sample = glm::normalize(sample);
            sample *= randomFloats(m_Rng);
            float scale = static_cast<float>(i) / 64.0f;
            scale = Lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            m_SSAOKernel.push_back(sample);
        }
    }

    void TestSSAO::CreateNoiseTexture() {
        std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
        std::vector<glm::vec3> ssaoNoise;
        ssaoNoise.reserve(16);
        for (unsigned int i = 0; i < 16; i++) {
            glm::vec3 noise(
                randomFloats(m_Rng) * 2.0f - 1.0f,
                randomFloats(m_Rng) * 2.0f - 1.0f,
                0.0f);
            ssaoNoise.push_back(noise);
        }

        if (m_NoiseTexture != 0) {
            GLCall(glDeleteTextures(1, &m_NoiseTexture));
            m_NoiseTexture = 0;
        }

        GLCall(glGenTextures(1, &m_NoiseTexture));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_NoiseTexture));
        GLCall(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB16F,
            4,
            4,
            0,
            GL_RGB,
            GL_FLOAT,
            ssaoNoise.data()));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}
