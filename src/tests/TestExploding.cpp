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
        m_Model = std::make_shared<Model>(FileUtil::shared().GetPath("./models/backpack/backpack.obj"));
    }

    void TestExploding::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
        m_Time += deltaTime;

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_Time * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = m_Camera.GetViewMatrix();
        auto fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 proj = glm::perspective((float)fov, 960.0f / 540.0f, 0.1f, 100.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", model);
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);
        m_Shader->SetUniform1f("time", m_Time);
    }

    void TestExploding::OnRender() {
        Test::OnRender();
        m_Model->Draw(*m_Shader);
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
