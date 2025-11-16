//
// Created by blueberry on 2025/11/16.
//

#include "TestExploding.h"

#include "../FileUtil.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestExploding::TestExploding() : m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)) {
        SetupCursorCallback();
        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_exploding.shader"));
        m_NormalShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_exploding_normal.shader"));
        m_Model = std::make_shared<Model>(FileUtil::shared().GetPath("./models/backpack/backpack.obj"));
    }

    void TestExploding::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
        m_Time += deltaTime;

        m_ModelMatrix = glm::mat4(1.0f);
        m_ViewMatrix = m_Camera.GetViewMatrix();
        auto fov = glm::radians(m_Camera.GetZoom());
        m_ProjectionMatrix = glm::perspective((float) fov, 960.0f / 540.0f, 0.1f, 100.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", m_ModelMatrix);
        m_Shader->SetUniformMat4f("view", m_ViewMatrix);
        m_Shader->SetUniformMat4f("projection", m_ProjectionMatrix);
        m_Shader->SetUniform1f("time", m_Time);


        m_NormalShader->Bind();
        m_NormalShader->SetUniformMat4f("view", m_ViewMatrix);
        m_NormalShader->SetUniformMat4f("projection", m_ProjectionMatrix);
    }

    void TestExploding::OnRender() {
        Test::OnRender();
        m_Model->Draw(*m_Shader);
        {
            auto model = glm::translate(m_ModelMatrix, glm::vec3(20.0f, 0.0f, 0.0f));
            m_NormalShader->Bind();
            m_NormalShader->SetUniformMat4f("model", model);
            m_Model->Draw(*m_NormalShader);
        }
    }

    void TestExploding::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestExploding::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestExploding::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}
