//
// Created by blueberry on 2025/10/4.
//

#include "TestAssimp.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {
    TestAssimp::TestAssimp():m_Camera(glm::vec3(0.0f,0.0f, 1.0f)) {
        m_Shader = std::make_shared<Shader>("../../res/shaders/assimp.shader");
        m_Model = std::make_shared<Model>("../../res/models/backpack/backpack.obj");
        SetupCursorCallback();
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestAssimp::~TestAssimp() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestAssimp::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        glm::mat4 model= glm::mat4(1.0f);
        glm::mat4 view = m_Camera.GetViewMatrix();

        auto fov = glm::radians(m_Camera.GetZoom());
        // std::cout<< "fov:" << fov << std::endl;
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 100.0);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("model", model);
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);
    }

    void TestAssimp::OnRender() {
        Test::OnRender();
        m_Model->Draw(*m_Shader);
    }

    void TestAssimp::OnImGuiRender() {
        Test::OnImGuiRender();
      //  ImGui::SliderFloat3("view",&m_EyePos.x, -100, 100);
    }

    void TestAssimp::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestAssimp::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestAssimp::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }


}
