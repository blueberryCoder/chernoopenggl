//
// Created by blueberry on 2024/7/5.
//

#pragma once

#include "Test.h"
#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

#include "glm/glm.hpp"

#include <memory>

namespace test {

    class TestAsteroid : public Test {
    public:
        TestAsteroid();

        ~TestAsteroid() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        void updateMatrices();

    private:
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Model> m_Model;
        Camera m_Camera;
    };
}
