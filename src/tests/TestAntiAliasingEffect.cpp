//
// Created by blueberry on 2025/10/27.
//

#include "TestAntiAliasingEffect.h"

#include <iostream>

#include "../FileUtil.h"
#include "../VertexBufferLayout.h"
#include "../WindowManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace {
    const float kCubeVertices[] = {
        // positions           // colors
        -0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
        0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
        0.5f, 0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
        0.5f, 0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,

        -0.5f, -0.5f, 0.5f, 0.2f, 0.6f, 0.8f,
        0.5f, -0.5f, 0.5f, 0.2f, 0.6f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.6f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.6f, 0.8f,
        -0.5f, 0.5f, 0.5f, 0.2f, 0.6f, 0.8f,
        -0.5f, -0.5f, 0.5f, 0.2f, 0.6f, 0.8f,

        -0.5f, 0.5f, 0.5f, 0.9f, 0.7f, 0.2f,
        -0.5f, 0.5f, -0.5f, 0.9f, 0.7f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.9f, 0.7f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.9f, 0.7f, 0.2f,
        -0.5f, -0.5f, 0.5f, 0.9f, 0.7f, 0.2f,
        -0.5f, 0.5f, 0.5f, 0.9f, 0.7f, 0.2f,

        0.5f, 0.5f, 0.5f, 0.2f, 0.8f, 0.3f,
        0.5f, 0.5f, -0.5f, 0.2f, 0.8f, 0.3f,
        0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 0.3f,
        0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 0.3f,
        0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.3f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.8f, 0.3f,

        -0.5f, -0.5f, -0.5f, 0.4f, 0.4f, 0.9f,
        0.5f, -0.5f, -0.5f, 0.4f, 0.4f, 0.9f,
        0.5f, -0.5f, 0.5f, 0.4f, 0.4f, 0.9f,
        0.5f, -0.5f, 0.5f, 0.4f, 0.4f, 0.9f,
        -0.5f, -0.5f, 0.5f, 0.4f, 0.4f, 0.9f,
        -0.5f, -0.5f, -0.5f, 0.4f, 0.4f, 0.9f,

        -0.5f, 0.5f, -0.5f, 0.8f, 0.5f, 0.8f,
        0.5f, 0.5f, -0.5f, 0.8f, 0.5f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.8f, 0.5f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.8f, 0.5f, 0.8f,
        -0.5f, 0.5f, 0.5f, 0.8f, 0.5f, 0.8f,
        -0.5f, 0.5f, -0.5f, 0.8f, 0.5f, 0.8f,
    };

    const unsigned int kCubeIndices[36] = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };

    const float kEffectQuadVertices[] = {
        // positions   // texcoords
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
    };

    const unsigned int kEffectQuadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };
}

namespace test {
    TestAntiAliasingEffect::TestAntiAliasingEffect() {
        SetupCursorCallback();
        m_WasDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
        m_WasMultisampleEnabled = glIsEnabled(GL_MULTISAMPLE);
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_MULTISAMPLE));

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_antialiasing.shader"));
        m_VAO = std::make_shared<VertexArray>();
        m_VBO = std::make_shared<VertexBuffer>(kCubeVertices, sizeof(kCubeVertices));
        m_IBO = std::make_shared<IndexBuffer>(kCubeIndices, 36);

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        m_VAO->AddBuffer(*m_VBO, layout);

        int width = 0;
        int height = 0;
        WindowManager::shared().GetFramebufferSize(width, height);
        if (width == 0 || height == 0) {
            width = 960;
            height = 540;
        }
        m_FboTexture = std::make_shared<Texture>(TextureInitParams{
            .type = GL_TEXTURE_2D_MULTISAMPLE,
            .samples = 4,
            .width = width,
            .height = height,
        });

        RenderBufferInitParams depthParams{};
        depthParams.width = width;
        depthParams.height = height;
        depthParams.samples = 4;
        m_DepthRbo = std::make_shared<RenderBuffer>(depthParams);

        m_FBO = std::make_shared<FrameBuffer>();
        m_FBO->AttachColor(m_FboTexture, 0);
        m_FBO->AttachDepthStencil(m_DepthRbo);
        GLCall(auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "MSAA Framebuffer incomplete, status=" << status << std::endl;
        }
        m_FBO->Unbind();

        m_EffectVAO = std::make_shared<VertexArray>();
        m_EffectVBO = std::make_shared<VertexBuffer>(kEffectQuadVertices, sizeof(kEffectQuadVertices));
        m_EffectIBO = std::make_shared<IndexBuffer>(kEffectQuadIndices, 6);

        VertexBufferLayout effectLayout{};
        effectLayout.Push<float>(2);
        effectLayout.Push<float>(2);
        m_EffectVAO->AddBuffer(*m_EffectVBO, effectLayout);

        m_EffectShader = std::make_shared<Shader>(
            FileUtil::shared().GetPath("./shaders/test_antialiasing_effect.shader"));
        m_EffectShader->Bind();
        m_EffectShader->SetUniform1i("u_Texture", 0);

        m_IntermediateTexture = std::make_shared<Texture>(TextureInitParams{
            .type = GL_TEXTURE_2D,
            .width = width,
            .height = height,
        });
        // GLCall(glBindTexture(GL_TEXTURE_2D, m_IntermediateTexture->GetID()));
        // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        // GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        m_IntermediateFBO = std::make_shared<FrameBuffer>();
        m_IntermediateFBO->AttachColor(m_IntermediateTexture, 0);
        GLCall(auto intermediateStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (intermediateStatus != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Intermediate framebuffer incomplete, status=" << intermediateStatus << std::endl;
        }
        m_IntermediateFBO->Unbind();
    }

    TestAntiAliasingEffect::~TestAntiAliasingEffect() {
        if (!m_WasDepthEnabled) {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
        if (!m_WasMultisampleEnabled) {
            GLCall(glDisable(GL_MULTISAMPLE));
        }
    }

    void TestAntiAliasingEffect::OnUpdate(float deltaTime) {
        // m_Rotation += deltaTime * m_RotationSpeed;
        // if (m_Rotation > glm::two_pi<float>()) {
        //     m_Rotation = 0.0f;
        // }

        int fbWidth = 0;
        int fbHeight = 0;
        WindowManager::shared().GetFramebufferSize(fbWidth, fbHeight);
        float aspect = (fbHeight != 0) ? static_cast<float>(fbWidth) / static_cast<float>(fbHeight) : (960.0f / 540.0f);

        glm::mat4 view = m_Camera->GetViewMatrix();
        float fov = glm::radians(m_Camera->GetZoom());
        glm::mat4 proj = glm::perspective(fov, aspect, 0.1f, 100.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);
    }

    void TestAntiAliasingEffect::OnRender() {
        int fbWidth = m_FboTexture ? m_FboTexture->GetWidth() : 0;
        int fbHeight = m_FboTexture ? m_FboTexture->GetHeight() : 0;
        if (fbWidth > 0 && fbHeight > 0) {
            GLCall(glViewport(0, 0, fbWidth, fbHeight));
        }

        GLCall(glClearColor(0.05f, 0.07f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.5f, 1.0f, 0.2f));
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", model);
        m_FBO->Bind();
        m_Renderer.Clear();
        m_Renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        m_FBO->Unbind();

        GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO->GetID()));
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermediateFBO->GetID()));
        auto width = m_FboTexture->GetWidth();
        auto height = m_FboTexture->GetHeight();
        GLCall(glBlitFramebuffer(0, 0, width, height,
            0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        m_IntermediateTexture->Bind();
        m_Renderer.Draw(*m_EffectVAO, *m_EffectIBO, *m_EffectShader);

    }

    void TestAntiAliasingEffect::OnImGuiRender() {
    }

    void TestAntiAliasingEffect::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestAntiAliasingEffect::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestAntiAliasingEffect::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }
}
