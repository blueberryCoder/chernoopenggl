//
// Created by blueberry on 2025/10/7.
//

#include "TestDepth.h"

#include "../FileUtil.h"
#include "../src/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestDepth::TestDepth() {
        SetupCursorCallback();

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_depth.shader"));
        m_CubeTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/marble.jpg"));
        m_FloorTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/metal.png"));

        m_CubeVBO = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]));

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, layout);

        m_PlaneVBO = std::make_shared<VertexBuffer>(planeVertices, sizeof(planeVertices));
        m_PlaneIBO = std::make_shared<IndexBuffer>(planeIndices, sizeof(planeIndices) / sizeof(planeIndices[0]));
        m_PlaneVAO = std::make_shared<VertexArray>();
        m_PlaneVAO->AddBuffer(*m_PlaneVBO, layout);

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

        m_Shader->Bind();
        m_Shader->SetUniform1i("texture1", 0);

        GLCall(glEnable(GL_DEPTH_TEST));
        m_DepthTexture = std::make_shared<Texture>(TextureInitParams{
            .width = 960,
            .height = 540,
            .internalFormat = GL_DEPTH_COMPONENT32F,
        });
        m_Fbo = std::make_shared<FrameBuffer>();
        m_Fbo->AttachDepth(m_DepthTexture);
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer is not complete!" << std::endl;
        }
        m_Fbo->Unbind();
        m_DepthVBO = std::make_shared<VertexBuffer>(depthVertices, sizeof(depthVertices));
        m_DepthIBO = std::make_shared<IndexBuffer>(depthIndices, 6);
        VertexBufferLayout depthLayout{};
        depthLayout.Push<float>(2);
        depthLayout.Push<float>(2);
        m_DepthVAO = std::make_shared<VertexArray>();
        m_DepthVAO->AddBuffer(*m_DepthVBO, depthLayout);

        m_DepthShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/draw_depth.shader"));
        m_DepthShader->Bind();
        m_DepthShader->SetUniform1i("u_Texture", 2);
        m_DepthShader->SetUniformMat4f("model", glm::mat4(1.0f));
        m_DepthShader->SetUniformMat4f(
            "view", glm::translate(glm::mat4(1.0f), glm::vec3(960 - 100.0f, 540.0 - 100.0f, 0.0f)));
        m_DepthShader->SetUniformMat4f("projection", glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f));

        m_CubeTexture->Bind();
        m_FloorTexture->Bind(1);
        m_DepthTexture->Bind(2);
    }

    TestDepth::~TestDepth() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestDepth::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        Test::ProcessInputEvent(window, deltaTime);
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestDepth::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }

    void TestDepth::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestDepth::OnImGuiRender() {
    }

    void TestDepth::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;
        m_Shader->Bind();
        m_Shader->SetUniform1i("texture1", 0);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, 0.0f, -1.0f));
        m_Shader->SetUniformMat4f("model", model);

        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);
        auto model2 = glm::translate(model, glm::vec3(0.5f, 0.0f, -3.0f));
        m_Shader->SetUniformMat4f("model", model2);
        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

        m_Shader->SetUniform1i("texture1", 1);

        renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, *m_Shader);

        // Copy depth buffer to fbo
        GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo->GetID()));
        GLCall(glBlitFramebuffer(0,0, 960, 540,
            0, 0, 960,540, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        m_DepthShader->Bind();
        m_DepthShader->SetUniform1i("u_Texture", 2);

        glDisable(GL_DEPTH_TEST);
        renderer.Draw(*m_DepthVAO, *m_DepthIBO, *m_DepthShader);
        glEnable(GL_DEPTH_TEST);
    }

    void TestDepth::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
        glm::mat4 view = m_Camera->GetViewMatrix();
        auto fov = glm::radians(m_Camera->GetZoom());
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 50.0);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);
    }
}
