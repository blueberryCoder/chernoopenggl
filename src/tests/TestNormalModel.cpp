//
// Created by blueberry on 2025/1/7.
//

#include "TestNormalModel.h"

#include "../FileUtil.h"
#include "../Renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {
    TestNormalModel::TestNormalModel()
        : m_Camera(glm::vec3(0.0f, 0.0f, 6.0f)),
          m_LightPos(2.0f, 2.0f, 2.0f),
          m_Blinn(true) {
        SetupCursorCallback();

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_normal_model.shader"));
        m_Model = std::make_shared<Model>(FileUtil::shared().GetPath("./models/cyborg/cyborg.obj"));

        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestNormalModel::~TestNormalModel() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestNormalModel::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        glm::mat4 view = m_Camera.GetViewMatrix();
        float fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 projection = glm::perspective<float>(fov, 960.0f / 540.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", model);
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", projection);
        m_Shader->SetUniformVec3f("lightPos", m_LightPos);
        m_Shader->SetUniformVec3f("viewPos", m_Camera.GetPosition());
        m_Shader->SetUniform1i("blinn", m_Blinn ? 1 : 0);
    }

    void TestNormalModel::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_Shader->Bind();
        m_Shader->SetUniform1i("blinn", m_Blinn ? 1 : 0);
        m_Model->Draw(*m_Shader);
    }

    void TestNormalModel::OnImGuiRender() {
        ImGui::Checkbox("Blinn-Phong", &m_Blinn);
        ImGui::DragFloat3("Light Pos", &m_LightPos.x, 0.05f);
    }

    void TestNormalModel::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestNormalModel::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestNormalModel::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}
