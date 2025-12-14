//
// Created by blueberry on 2025/1/2.
//

#pragma once

#include "Test.h"
#include "../Camera.h"
#include "../IndexBuffer.h"
#include "../Renderer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

#include <glm/glm.hpp>
#include <memory>

namespace test {

    class TestAdvancedLighting : public Test {
    public:
        TestAdvancedLighting();

        ~TestAdvancedLighting() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        Camera m_Camera;
        glm::vec3 m_LightPos;
        bool m_Blinn;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Texture> m_FloorTexture;
        std::shared_ptr<VertexArray> m_VAO;
        std::shared_ptr<VertexBuffer> m_VBO;
        std::shared_ptr<IndexBuffer> m_IBO;

        float m_PlaneVertices[48] = {
            // positions            // normals         // texcoords
            10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
            -10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

            10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
            -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
            10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f
        };

        unsigned int m_PlaneIndices[6] = {0, 1, 2, 3, 4, 5};
    };
}
