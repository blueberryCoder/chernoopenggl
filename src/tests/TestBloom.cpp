//
// Created by blueberry on 2026/2/1.
//

#include "TestBloom.h"

#include "../FileUtil.h"
#include "../WindowManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <iostream>
#include <string>

namespace test {
    TestBloom::TestBloom() {
        SetupCursorCallback();
        m_WasDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLCall(glEnable(GL_DEPTH_TEST));

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 8.0f));

        m_SceneShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_bloom.shader"));
        m_BlurShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_blur.shader"));
        m_FinalShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_bloom_final.shader"));

        m_TexturePrimary = std::make_shared<Texture>(
            FileUtil::shared().GetPath("./textures/wood.png"));
        m_TextureSecondary = std::make_shared<Texture>(
            FileUtil::shared().GetPath("./textures/container2.png"));

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

        TextureInitParams colorParams{};
        colorParams.WRAP_S = GL_CLAMP_TO_EDGE;
        colorParams.WRAP_T = GL_CLAMP_TO_EDGE;
        colorParams.MIN_FILTER = GL_LINEAR;
        colorParams.MAG_FILTER = GL_LINEAR;
        colorParams.width = width;
        colorParams.height = height;
        colorParams.internalFormat = GL_RGB16F;

        m_ColorBuffer = std::make_shared<Texture>(colorParams);
        m_BrightBuffer = std::make_shared<Texture>(colorParams);

        RenderBufferInitParams depthParams{};
        depthParams.width = width;
        depthParams.height = height;
        m_DepthRbo = std::make_shared<RenderBuffer>(depthParams);

        m_SceneFbo = std::make_shared<FrameBuffer>();
        m_SceneFbo->AttachColor(m_ColorBuffer, 0);
        m_SceneFbo->AttachColor(m_BrightBuffer, 1);
        m_SceneFbo->AttachDepthStencil(m_DepthRbo);
        GLenum attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        GLCall(glDrawBuffers(2, attachments));
        GLCall(auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Bloom framebuffer incomplete, status=" << status << std::endl;
        }
        m_SceneFbo->Unbind();

        for (int i = 0; i < 2; ++i) {
            m_PingpongColor[i] = std::make_shared<Texture>(colorParams);
            m_PingpongFbo[i] = std::make_shared<FrameBuffer>();
            m_PingpongFbo[i]->AttachColor(m_PingpongColor[i], 0);
            GLCall(auto pingpongStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
            if (pingpongStatus != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "Pingpong framebuffer incomplete, status=" << pingpongStatus << std::endl;
            }
            m_PingpongFbo[i]->Unbind();
        }

        m_SceneShader->Bind();
        m_SceneShader->SetUniform1i("diffuseTexture", 0);
        m_SceneShader->SetUniform1i("useEmissive", 0);
        m_SceneShader->Unbind();

        m_BlurShader->Bind();
        m_BlurShader->SetUniform1i("image", 0);
        m_BlurShader->Unbind();

        m_FinalShader->Bind();
        m_FinalShader->SetUniform1i("scene", 0);
        m_FinalShader->SetUniform1i("bloomBlur", 1);
        m_FinalShader->Unbind();
    }

    TestBloom::~TestBloom() {
        if (!m_WasDepthEnabled) {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
    }

    void TestBloom::OnUpdate(float deltaTime) {
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

        m_SceneShader->Bind();
        m_SceneShader->SetUniformMat4f("view", view);
        m_SceneShader->SetUniformMat4f("projection", projection);
        m_SceneShader->SetUniformVec3f("viewPos", m_Camera->GetPosition());

        for (int i = 0; i < 4; ++i) {
            m_SceneShader->SetUniformVec3f("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
            m_SceneShader->SetUniformVec3f("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
        }
    }

    void TestBloom::OnRender() {
        GLCall(glEnable(GL_DEPTH_TEST));

        int fboWidth = m_ColorBuffer ? m_ColorBuffer->GetWidth() : 0;
        int fboHeight = m_ColorBuffer ? m_ColorBuffer->GetHeight() : 0;
        if (fboWidth > 0 && fboHeight > 0) {
            GLCall(glViewport(0, 0, fboWidth, fboHeight));
        }

        m_SceneFbo->Bind();
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_SceneShader->Bind();
        m_SceneShader->SetUniform1i("useEmissive", 0);

        m_TexturePrimary->Bind(0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(12.0f, 0.5f, 12.0f));
        m_SceneShader->SetUniformMat4f("model", model);
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_SceneShader);

        m_TextureSecondary->Bind(0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        m_SceneShader->SetUniformMat4f("model", model);
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_SceneShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        m_SceneShader->SetUniformMat4f("model", model);
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_SceneShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0f));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
        m_SceneShader->SetUniformMat4f("model", model);
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_SceneShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.75f));
        m_SceneShader->SetUniformMat4f("model", model);
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_SceneShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0f));
        model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
        model = glm::scale(model, glm::vec3(1.25f));
        m_SceneShader->SetUniformMat4f("model", model);
        m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_SceneShader);

        m_SceneShader->SetUniform1i("useEmissive", 1);
        for (int i = 0; i < 4; ++i) {
            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, m_LightPositions[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.25f));
            m_SceneShader->SetUniformMat4f("model", lightModel);
            m_SceneShader->SetUniformVec3f("emissiveColor", m_LightColors[i]);
            m_Renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_SceneShader);
        }
        m_SceneShader->SetUniform1i("useEmissive", 0);

        m_SceneFbo->Unbind();

        GLCall(glDisable(GL_DEPTH_TEST));

        bool horizontal = true;
        bool firstIteration = true;
        int blurPasses = m_BlurPasses > 0 ? m_BlurPasses : 1;

        if (fboWidth > 0 && fboHeight > 0) {
            GLCall(glViewport(0, 0, fboWidth, fboHeight));
        }

        m_BlurShader->Bind();
        for (int i = 0; i < blurPasses; ++i) {
            m_PingpongFbo[horizontal ? 1 : 0]->Bind();
            m_BlurShader->SetUniform1i("horizontal", horizontal ? 1 : 0);

            if (firstIteration) {
                m_BrightBuffer->Bind(0);
                firstIteration = false;
            } else {
                m_PingpongColor[horizontal ? 0 : 1]->Bind(0);
            }

            m_Renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_BlurShader);
            horizontal = !horizontal;
        }
        m_PingpongFbo[0]->Unbind();

        int viewportWidth = 0;
        int viewportHeight = 0;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);
        if (viewportWidth <= 0 || viewportHeight <= 0) {
            viewportWidth = 960;
            viewportHeight = 540;
        }
        GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_FinalShader->Bind();
        m_FinalShader->SetUniform1i("bloom", m_BloomEnabled ? 1 : 0);
        m_FinalShader->SetUniform1f("exposure", m_Exposure);
        m_ColorBuffer->Bind(0);
        m_PingpongColor[horizontal ? 0 : 1]->Bind(1);
        m_Renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_FinalShader);
    }

    void TestBloom::OnImGuiRender() {
        ImGui::Checkbox("Bloom", &m_BloomEnabled);
        ImGui::SliderFloat("Exposure", &m_Exposure, 0.1f, 5.0f);
        ImGui::SliderInt("Blur passes", &m_BlurPasses, 1, 20);
    }

    void TestBloom::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestBloom::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestBloom::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }
}
