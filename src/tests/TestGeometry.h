//
// Created by blueberry on 2025/11/16.
//

#pragma once

#include "Test.h"
#include <memory>
#include "../Camera.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

namespace test {
    class TestGeometry : public Test {
    public:
        TestGeometry();

        ~TestGeometry() override = default;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VAO;
        std::shared_ptr<VertexBuffer> m_VBO;
        std::shared_ptr<IndexBuffer> m_IBO;
        Camera m_Camera;
        float m_Rotation = 0.0f;
    };
}
