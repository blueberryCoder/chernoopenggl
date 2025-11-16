//
// Created by blueberry on 2025/11/16.
//

#include "TestGeometry.h"

#include "../FileUtil.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Renderer.h"

namespace test {
    TestGeometry::TestGeometry() : m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)) {
        SetupCursorCallback();

        float vertices[] = {
                // positions        // colors
                -0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.1f,
                0.5f, -0.5f, 0.0f,  0.1f, 0.8f, 0.1f,
                0.5f, 0.5f, 0.0f,   0.1f, 0.1f, 0.8f,
                -0.5f, 0.5f, 0.0f,  0.9f, 0.9f, 0.2f
        };
        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        m_VAO = std::make_shared<VertexArray>();
        m_VBO = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        m_VAO->AddBuffer(*m_VBO, layout);
        m_IBO = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_geometry_house.shader"));
    }

    void TestGeometry::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
        // m_Rotation += deltaTime * 0.5f;
        //
        // glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 view = m_Camera.GetViewMatrix();
        auto fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 projection = glm::perspective((float)fov, 960.0f / 540.0f, 0.1f, 100.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", glm::mat4(1.0f));
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", projection);
    }

    void TestGeometry::OnRender() {
        Renderer renderer;
        renderer.Clear();
        m_Shader->Bind();

        {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("model", glm::translate(glm::mat4(1.0),
                glm::vec3(-2.5, 0, 0)));
            renderer.DrawPoints(*m_VAO, *m_IBO, *m_Shader);

        }

        {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("model", glm::translate(glm::mat4(1.0),
                glm::vec3(2.5, 0, 0)));
            renderer.DrawPoints(*m_VAO, *m_IBO, *m_Shader);
        }

    }

    void TestGeometry::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestGeometry::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestGeometry::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}
