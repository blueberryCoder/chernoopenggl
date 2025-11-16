//
// Created by blueberry on 2025/11/16.
//

#include "TestAdvanced.h"

#include "../FileUtil.h"
#include  "../src/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
namespace test {
    TestAdvanced::TestAdvanced()  :m_Camera(glm::vec3(0.0f,0.0f, 1.0f)) {
        GLCall(glPointSize(20.0f));

        m_PointShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_point_size.shader"));
        m_PointVBO = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
        m_PointIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices)/ sizeof(cubeIndices[0]));

        VertexBufferLayout layout = {};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_PointVAO = std::make_shared<VertexArray>();
        m_PointVAO->AddBuffer(*m_PointVBO,layout);

        SetupCursorCallback();
        m_PointShader->Bind();
        m_PointShader->SetUniformMat4f("view", glm::mat4(1.0f));
        m_PointShader->SetUniformMat4f("projection", glm::mat4(1.0f));
        m_PointShader->SetUniformMat4f("model", glm::mat4(1.0f));
    };

    TestAdvanced::~TestAdvanced() {
        // reset gl point size
    };

    void TestAdvanced::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        glm::mat4 view = m_Camera.GetViewMatrix();
        auto fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 100.0);
        m_PointShader->Bind();
        m_PointShader->SetUniformMat4f("view", view);
        m_PointShader->SetUniformMat4f("projection", proj);
    };

    void TestAdvanced::OnRender() {
        Renderer renderer;
        renderer.Clear();
        m_PointShader->Bind();
        m_PointShader->SetUniformMat4f("model", glm::mat4(1.0f));
        renderer.DrawPoints(*m_PointVAO, *m_PointIBO, *m_PointShader);
    };

    void TestAdvanced::OnImGuiRender() {};

    void TestAdvanced::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestAdvanced::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestAdvanced::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}