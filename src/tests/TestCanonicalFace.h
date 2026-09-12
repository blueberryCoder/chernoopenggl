//
// Created by blueberry on 2026/9/12.
//

#pragma once

#include "Test.h"

#include "../Camera.h"
#include "../Model.h"
#include "../Shader.h"

namespace test {

class TestCanonicalFace : public Test {
public:
    TestCanonicalFace();
    ~TestCanonicalFace() override;

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
    glm::vec3 m_ModelColor;
    glm::vec3 m_LightDir;
    float m_Scale;
    bool m_Wireframe;
};

}
