//
// Created by blueberry on 2026/3/21.
//

#pragma once

#include "Test.h"
#include "../Camera.h"
#include "../IndexBuffer.h"
#include "../Renderer.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace test {
    class TestPBR : public Test {
    public:
        TestPBR();

        ~TestPBR() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        void BuildSphereMesh();

        Camera m_Camera;
        Renderer m_Renderer;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_SphereVAO;
        std::shared_ptr<VertexBuffer> m_SphereVBO;
        std::shared_ptr<IndexBuffer> m_SphereIBO;
        std::vector<float> m_SphereVertices;
        std::vector<unsigned int> m_SphereIndices;

        glm::vec3 m_Albedo = glm::vec3(0.5f, 0.0f, 0.0f);
        float m_Ao = 1.0f;
        int m_RowCount = 7;
        int m_ColumnCount = 7;
        float m_Spacing = 2.5f;
        float m_SphereScale = 0.5f;
        bool m_WasDepthEnabled = false;

        glm::vec3 m_LightPositions[4] = {
            glm::vec3(-10.0f, 10.0f, 10.0f),
            glm::vec3(10.0f, 10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3(10.0f, -10.0f, 10.0f)
        };

        glm::vec3 m_LightColors[4] = {
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f)
        };
    };
}
