//
// Created by blueberry on 2025/10/7.
//

#include "TestFaceCulling.h"

#include "../FileUtil.h"
#include "../src/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestFaceCulling::TestFaceCulling() {
        SetupCursorCallback();
        GLCall(glEnable(GL_CULL_FACE));
        glCullFace(GL_BACK);

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_culling.shader"));
        m_CubeTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/marble.jpg"));

        m_CubeVBO = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]));

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, layout);

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Shader->Bind();
        m_Shader->SetUniform1i("texture1", 0);

        GLCall(glEnable(GL_DEPTH_TEST));

        m_CubeTexture->Bind();
    }

    TestFaceCulling::~TestFaceCulling() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestFaceCulling::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestFaceCulling::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }

    void TestFaceCulling::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestFaceCulling::OnImGuiRender() {
    }

    void TestFaceCulling::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));


        Renderer renderer;
        m_Shader->Bind();

        // Draw 2 cube.
        m_Shader->SetUniform1i("texture1", 0);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, 0.0f, -1.0f));
        m_Shader->SetUniformMat4f("model", model);
        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

        auto model2 = glm::translate(model, glm::vec3(0.5f, 0.0f, -3.0f));
        m_Shader->SetUniformMat4f("model", model2);
        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

    }

    void TestFaceCulling::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
        glm::mat4 view = m_Camera->GetViewMatrix();
        auto fov = glm::radians(m_Camera->GetZoom());
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 50.0);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);
    }
}
