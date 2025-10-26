//
// Created by blueberry on 2025/10/26.
//

#pragma once
#include "Test.h"
#include "../Camera.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexArray.h"

namespace test {
    class TestCubeMap : public Test {
    public:
        TestCubeMap();
        ~TestCubeMap() override;
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;
        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;
        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
         float cubeVertices[36 * 5] = {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left

            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left

            // Left face
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right

            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right

            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left

            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f // bottom-left
        };


        unsigned int cubeIndices[36] = {
            0, 1, 2, 3, 4, 5,
            6, 7, 8, 9, 10, 11,
            12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23,
            24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35
        };

        float skyboxVertices[3 * 36] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        unsigned int skyboxIndices[36] = {
            0, 1, 2, 3, 4, 5,
            6, 7, 8, 9, 10, 11,
            12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23,
            24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35
        };

        std::shared_ptr<Shader> m_Shader, m_SkyboxShader;
        std::shared_ptr<VertexArray> m_CubeVAO, m_SkyboxVAO;
        std::shared_ptr<VertexBuffer> m_CubeVBO, m_SkyboxVBO;
        std::shared_ptr<IndexBuffer> m_CubeIBO, m_SkyboxIBO;
        std::shared_ptr<Texture> m_CubeTexture, m_SkyboxTexture;

        std::shared_ptr<Camera> m_Camera;
        Renderer renderer;

    };
}
