//
// Created by blueberry on 2026/1/31.
//

#include "TestHDR.h"

#include "../FileUtil.h"
#include "../WindowManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <iostream>
#include <string>

namespace test {
    TestHDR::TestHDR() {
        SetupCursorCallback();
        m_WasDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLCall(glEnable(GL_DEPTH_TEST));

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 5.0f));

        m_LightingShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_lighting.shader"));
        m_HdrShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_hdr.shader"));

        m_WoodTexture = std::make_shared<Texture>(
            FileUtil::shared().GetPath("./textures/wood.png"));

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

        m_ColorBuffer = std::make_shared<Texture>(TextureInitParams{
            .WRAP_S = GL_CLAMP_TO_EDGE,
            .WRAP_T = GL_CLAMP_TO_EDGE,
            .MIN_FILTER = GL_LINEAR,
            .MAG_FILTER = GL_LINEAR,
            .width = width,
            .height = height,
            .internalFormat = GL_RGB16F,
        });

        RenderBufferInitParams depthParams{};
        depthParams.width = width;
        depthParams.height = height;
        m_DepthRbo = std::make_shared<RenderBuffer>(depthParams);

        m_HdrFBO = std::make_shared<FrameBuffer>();
        m_HdrFBO->AttachColor(m_ColorBuffer, 0);
        m_HdrFBO->AttachDepthStencil(m_DepthRbo);
        GLCall(auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "HDR framebuffer incomplete, status=" << status << std::endl;
        }
        m_HdrFBO->Unbind();

        m_LightingShader->Bind();
        m_LightingShader->SetUniform1i("diffuseTexture", 0);
        m_LightingShader->SetUniform1i("inverse_normals", 0);
        m_LightingShader->Unbind();

        m_HdrShader->Bind();
        m_HdrShader->SetUniform1i("hdrBuffer", 0);
        m_HdrShader->Unbind();
    }

    TestHDR::~TestHDR() {
        if (!m_WasDepthEnabled) {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
    }

    void TestHDR::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        int fbWidth = 0;
        int fbHeight = 0;
        WindowManager::shared().GetFramebufferSize(fbWidth, fbHeight);
        float aspect = (fbHeight != 0)
            ? static_cast<float>(fbWidth) / static_cast<float>(fbHeight)
            : (960.0f / 540.0f);

        glm::mat4 view = m_Camera->GetViewMatrix();
        float fov = glm::radians(m_Camera->GetZoom());
        glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

        m_LightingShader->Bind();
        m_LightingShader->SetUniformMat4f("view", view);
        m_LightingShader->SetUniformMat4f("projection", projection);
        m_LightingShader->SetUniformVec3f("viewPos", m_Camera->GetPosition());

        for (int i = 0; i < 16; ++i) {
            glm::vec3 position(0.0f);
            glm::vec3 color(0.0f);
            if (i < 4) {
                position = m_LightPositions[i];
                color = m_LightColors[i];
            }
            m_LightingShader->SetUniformVec3f("lights[" + std::to_string(i) + "].Position", position);
            m_LightingShader->SetUniformVec3f("lights[" + std::to_string(i) + "].Color", color);
        }
    }

    void TestHDR::OnRender() {
        int fbWidth = m_ColorBuffer ? m_ColorBuffer->GetWidth() : 0;
        int fbHeight = m_ColorBuffer ? m_ColorBuffer->GetHeight() : 0;
        if (fbWidth > 0 && fbHeight > 0) {
            GLCall(glViewport(0, 0, fbWidth, fbHeight));
        }

        m_HdrFBO->Bind();
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_LightingShader->Bind();
        m_LightingShader->SetUniform1i("inverse_normals", 1);
        m_WoodTexture->Bind(0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
        m_LightingShader->SetUniformMat4f("model", model);
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_LightingShader);

        m_HdrFBO->Unbind();

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);
        if (viewportWidth <= 0 || viewportHeight <= 0) {
            viewportWidth = 960;
            viewportHeight = 540;
        }
        GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_HdrShader->Bind();
        m_HdrShader->SetUniform1i("hdr", m_HdrEnabled ? 1 : 0);
        m_HdrShader->SetUniform1f("exposure", m_Exposure);
        m_ColorBuffer->Bind(0);
        m_Renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_HdrShader);
    }

    void TestHDR::OnImGuiRender() {
        ImGui::Checkbox("HDR", &m_HdrEnabled);
        ImGui::SliderFloat("Exposure", &m_Exposure, 0.1f, 5.0f);
    }

    void TestHDR::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestHDR::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestHDR::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }
}
