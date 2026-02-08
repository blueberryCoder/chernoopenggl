//
// Created by blueberry on 2026/2/8.
//

#include "TestDeferredShadingVolums.h"

#include "../FileUtil.h"
#include "../WindowManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace test {
    TestDeferredShadingVolums::TestDeferredShadingVolums() {
        SetupCursorCallback();
        m_WasDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLCall(glEnable(GL_DEPTH_TEST));

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 8.0f));
        m_Model = std::make_shared<Model>(FileUtil::shared().GetPath("./models/backpack/backpack.obj"));

        m_GBufferShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_gbuffer.shader"));
        m_AmbientShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_deferred_ambient.shader"));
        m_LightVolumeShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_deferred_light_volume.shader"));
        m_LightBoxShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_deferred_lightbox.shader"));

        m_CubeVBO = std::make_shared<VertexBuffer>(m_CubeVertices, sizeof(m_CubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(m_CubeIndices, 36);
        VertexBufferLayout cubeLayout{};
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(2);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);

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

        m_GBufferShader->Bind();
        m_GBufferShader->SetUniform1i("material.texture_diffuse1", 0);
        m_GBufferShader->SetUniform1i("material.texture_specular1", 1);
        m_GBufferShader->Unbind();

        m_AmbientShader->Bind();
        m_AmbientShader->SetUniform1i("gAlbedoSpec", 0);
        m_AmbientShader->Unbind();

        m_LightVolumeShader->Bind();
        m_LightVolumeShader->SetUniform1i("gPosition", 0);
        m_LightVolumeShader->SetUniform1i("gNormal", 1);
        m_LightVolumeShader->SetUniform1i("gAlbedoSpec", 2);
        m_LightVolumeShader->Unbind();

        m_ObjectPositions = {
            glm::vec3(-3.0f, -1.0f, -3.0f),
            glm::vec3(0.0f, -1.0f, -3.0f),
            glm::vec3(3.0f, -1.0f, -3.0f),
            glm::vec3(-3.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(3.0f, -1.0f, 0.0f),
            glm::vec3(-3.0f, -1.0f, 3.0f),
            glm::vec3(0.0f, -1.0f, 3.0f),
            glm::vec3(3.0f, -1.0f, 3.0f)
        };

        m_LightPositions.reserve(kLightCount);
        m_LightColors.reserve(kLightCount);
        for (int z = 0; z < 2; ++z) {
            for (int y = 0; y < 4; ++y) {
                for (int x = 0; x < 4; ++x) {
                    glm::vec3 position(
                        (static_cast<float>(x) - 1.5f) * 3.0f,
                        (static_cast<float>(y) - 1.5f) * 2.0f,
                        (static_cast<float>(z) - 0.5f) * 6.0f);
                    m_LightPositions.push_back(position);

                    glm::vec3 color(
                        0.5f + 0.5f * (static_cast<float>(x) / 3.0f),
                        0.5f + 0.5f * (static_cast<float>(y) / 3.0f),
                        0.5f + 0.5f * static_cast<float>(z));
                    m_LightColors.push_back(color);
                }
            }
        }
    }

    TestDeferredShadingVolums::~TestDeferredShadingVolums() {
        if (!m_WasDepthEnabled) {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
    }

    void TestDeferredShadingVolums::OnUpdate(float deltaTime) {
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

        m_GBufferShader->Bind();
        m_GBufferShader->SetUniformMat4f("view", m_View);
        m_GBufferShader->SetUniformMat4f("projection", m_Projection);

        m_AmbientShader->Bind();
        m_AmbientShader->SetUniform1f("ambientStrength", m_AmbientStrength);

        m_LightVolumeShader->Bind();
        m_LightVolumeShader->SetUniformMat4f("view", m_View);
        m_LightVolumeShader->SetUniformMat4f("projection", m_Projection);
        m_LightVolumeShader->SetUniformVec3f("viewPos", m_Camera->GetPosition());
        m_LightVolumeShader->SetUniformVec2f("screenSize", glm::vec2(
            static_cast<float>(m_GBufferWidth), static_cast<float>(m_GBufferHeight)));

        m_LightBoxShader->Bind();
        m_LightBoxShader->SetUniformMat4f("view", m_View);
        m_LightBoxShader->SetUniformMat4f("projection", m_Projection);

        m_GBufferShader->Unbind();
        m_AmbientShader->Unbind();
        m_LightVolumeShader->Unbind();
        m_LightBoxShader->Unbind();
    }

    void TestDeferredShadingVolums::OnRender() {
        int fbWidth = m_GBufferWidth;
        int fbHeight = m_GBufferHeight;
        if (fbWidth > 0 && fbHeight > 0) {
            GLCall(glViewport(0, 0, fbWidth, fbHeight));
        }

        m_GBuffer->Bind();
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_GBufferShader->Bind();
        for (const auto &pos : m_ObjectPositions) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(0.5f));
            m_GBufferShader->SetUniformMat4f("model", model);
            m_Model->Draw(*m_GBufferShader);
        }
        m_GBuffer->Unbind();

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);
        if (viewportWidth <= 0 || viewportHeight <= 0) {
            viewportWidth = 960;
            viewportHeight = 540;
        }
        GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glDisable(GL_DEPTH_TEST));

        m_AmbientShader->Bind();
        m_GAlbedoSpec->Bind(0);
        m_Renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_AmbientShader);

        GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetID()));
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
        GLCall(glBlitFramebuffer(
            0, 0, m_GBufferWidth, m_GBufferHeight,
            0, 0, viewportWidth, viewportHeight,
            GL_DEPTH_BUFFER_BIT, GL_NEAREST));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_ONE, GL_ONE));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthMask(GL_FALSE));
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));

        m_LightVolumeShader->Bind();
        m_GPosition->Bind(0);
        m_GNormal->Bind(1);
        m_GAlbedoSpec->Bind(2);

        for (size_t i = 0; i < m_LightPositions.size(); ++i) {
            const auto &position = m_LightPositions[i];
            const auto &color = m_LightColors[i];
            float radius = ComputeLightRadius(color);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::scale(model, glm::vec3(radius));
            m_LightVolumeShader->SetUniformMat4f("model", model);
            m_LightVolumeShader->SetUniformVec3f("light.Position", position);
            m_LightVolumeShader->SetUniformVec3f("light.Color", color);
            m_LightVolumeShader->SetUniform1f("light.Linear", m_LightLinear);
            m_LightVolumeShader->SetUniform1f("light.Quadratic", m_LightQuadratic);
            m_LightVolumeShader->SetUniform1f("light.Radius", radius);

            m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_LightVolumeShader);
        }

        GLCall(glCullFace(GL_BACK));
        GLCall(glDisable(GL_CULL_FACE));
        GLCall(glDepthMask(GL_TRUE));
        GLCall(glDisable(GL_BLEND));

        m_LightBoxShader->Bind();
        for (size_t i = 0; i < m_LightPositions.size(); ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_LightPositions[i]);
            model = glm::scale(model, glm::vec3(0.15f));
            m_LightBoxShader->SetUniformMat4f("model", model);
            m_LightBoxShader->SetUniformVec3f("lightColor", m_LightColors[i]);
            m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_LightBoxShader);
        }
    }

    void TestDeferredShadingVolums::OnImGuiRender() {
        ImGui::Text("Deferred shading (light volumes)");
        ImGui::SliderFloat("Ambient", &m_AmbientStrength, 0.0f, 1.0f);
        ImGui::Text("Lights: %zu", m_LightPositions.size());
    }

    void TestDeferredShadingVolums::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestDeferredShadingVolums::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestDeferredShadingVolums::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }

    void TestDeferredShadingVolums::CreateGBuffer(int width, int height) {
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
        albedoParams.internalFormat = GL_RGBA16;

        m_GPosition = std::make_shared<Texture>(positionParams);
        m_GNormal = std::make_shared<Texture>(normalParams);
        m_GAlbedoSpec = std::make_shared<Texture>(albedoParams);

        RenderBufferInitParams depthParams{};
        depthParams.width = width;
        depthParams.height = height;
        m_DepthRbo = std::make_shared<RenderBuffer>(depthParams);

        m_GBuffer = std::make_shared<FrameBuffer>();
        m_GBuffer->AttachColor(m_GPosition, 0);
        m_GBuffer->AttachColor(m_GNormal, 1);
        m_GBuffer->AttachColor(m_GAlbedoSpec, 2);
        m_GBuffer->AttachDepthStencil(m_DepthRbo);

        GLenum attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        GLCall(glDrawBuffers(3, attachments));
        GLCall(auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "GBuffer framebuffer incomplete, status=" << status << std::endl;
        }
        m_GBuffer->Unbind();
    }

    float TestDeferredShadingVolums::ComputeLightRadius(const glm::vec3 &color) const {
        float maxBrightness = std::max(color.r, std::max(color.g, color.b));
        float constant = 1.0f;
        float term = m_LightLinear * m_LightLinear
            - 4.0f * m_LightQuadratic * (constant - (256.0f / 5.0f) * maxBrightness);
        if (term < 0.0f) {
            return 0.0f;
        }
        return (-m_LightLinear + std::sqrt(term)) / (2.0f * m_LightQuadratic);
    }
}
