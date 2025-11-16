//
// Created by blueberry on 2025/11/16.
//

#pragma once

#include "Test.h"
#include <memory>
#include "../Camera.h"
#include "../Shader.h"
#include "../Model.h"

namespace test {
    class TestExploding : public Test {
    public:
        TestExploding();
        ~TestExploding() override = default;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override {}

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;
        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;
        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Shader> m_NormalShader;
        std::shared_ptr<Model> m_Model;
        Camera m_Camera;
        float m_Time = 0.0f;
        glm::mat4 m_ModelMatrix{1.0f};
        glm::mat4 m_ViewMatrix{1.0f};
        glm::mat4 m_ProjectionMatrix{1.0f};
    };
}
