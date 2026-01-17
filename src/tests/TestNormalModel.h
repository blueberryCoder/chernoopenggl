//
// Created by blueberry on 2025/1/7.
//

#pragma once

#include "Test.h"
#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

#include <memory>
#include <glm/glm.hpp>

namespace test {
    class TestNormalModel : public Test {
    public:
        TestNormalModel();

        ~TestNormalModel() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Model> m_Model;
        Camera m_Camera;
        glm::vec3 m_LightPos;
        bool m_Blinn;
    };
}
