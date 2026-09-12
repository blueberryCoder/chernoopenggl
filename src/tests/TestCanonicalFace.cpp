//
// Created by blueberry on 2026/9/12.
//

#include "TestCanonicalFace.h"

#include "../FileUtil.h"
#include "../Renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {
    TestCanonicalFace::TestCanonicalFace()
        : m_Camera(glm::vec3(0.0f, 0.0f, 20.0f)),
          m_ModelColor(0.82f, 0.68f, 0.56f),
          m_LightDir(-0.35f, -0.55f, -1.0f),
          m_Scale(0.75f),
          m_Wireframe(false) {
        SetupCursorCallback();

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/canonical_face.shader"));
        m_Model = std::make_shared<Model>(FileUtil::shared().GetPath("./models/canoical/canonical_face_model.obj"));

        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestCanonicalFace::~TestCanonicalFace() {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestCanonicalFace::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));
        model = glm::scale(model, glm::vec3(m_Scale));

        glm::mat4 view = m_Camera.GetViewMatrix();
        float fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 projection = glm::perspective<float>(fov, 960.0f / 540.0f, 0.1f, 100.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", model);
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", projection);
        m_Shader->SetUniformVec3f("modelColor", m_ModelColor);
        m_Shader->SetUniformVec3f("lightDir", glm::normalize(m_LightDir));
    }

    void TestCanonicalFace::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, m_Wireframe ? GL_LINE : GL_FILL));
        m_Model->Draw(*m_Shader);
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

    void TestCanonicalFace::OnImGuiRender() {
        ImGui::ColorEdit3("Model Color", &m_ModelColor.x);
        ImGui::DragFloat3("Light Dir", &m_LightDir.x, 0.02f, -1.0f, 1.0f);
        ImGui::SliderFloat("Scale", &m_Scale, 0.1f, 2.0f);
        ImGui::Checkbox("Wireframe", &m_Wireframe);
    }

    void TestCanonicalFace::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestCanonicalFace::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestCanonicalFace::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}
