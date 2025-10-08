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

        m_Camera = std::make_shared<Camera>();

        m_Shader->Bind();
        m_Shader->SetUniform1i("texture1", 0);
        m_CubeTexture->Bind();

        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestDepth::~TestDepth() {
    }

    void TestDepth::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
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
        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);
    }

    void TestDepth::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_Camera->GetViewMatrix();


        auto fov = glm::radians(m_Camera->GetZoom());
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 100.0);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", model);
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);
    }
}
