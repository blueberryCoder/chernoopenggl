//
// Created by blueberry on 2024/7/5.
//

#include "TestAsteroid.h"
#include "../FileUtil.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {

    namespace {
        constexpr float kDefaultAspect = 960.0f / 540.0f;
    }

    TestAsteroid::TestAsteroid()
            : m_Camera(glm::vec3(0.0f, 0.0f, 5.0f))
    {
        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/assimp.shader"));
        m_Model = std::make_shared<Model>(FileUtil::shared().GetPath("./models/planet/planet.obj"));
        SetupCursorCallback();
    }

    TestAsteroid::~TestAsteroid() = default;

    void TestAsteroid::OnUpdate(float deltaTime) {

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

    void TestAsteroid::OnRender() {
        if (m_Model && m_Shader) {
            m_Model->Draw(*m_Shader);
        }
    }

    void TestAsteroid::OnImGuiRender() {
    }

    void TestAsteroid::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestAsteroid::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestAsteroid::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }

    void TestAsteroid::updateMatrices() {




    }
}
