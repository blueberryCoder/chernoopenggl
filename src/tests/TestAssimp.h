//
// Created by blueberry on 2025/10/4.
//

#pragma once
#include "Test.h"

#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

namespace test {


class TestAssimp : public Test {
public:
    TestAssimp();
    ~TestAssimp() override;

    void OnUpdate(float deltaTime) override;

    void OnRender() override;

    void OnImGuiRender() override;

    void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

    void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

    void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

private:
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Model> m_Model;
    // glm::vec3 m_EyePos;
    Camera m_Camera;
};

}
