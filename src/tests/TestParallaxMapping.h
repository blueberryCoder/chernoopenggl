//
// Created by blueberry on 2026/01/24.
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
    class TestParallaxMapping : public Test {
    public:
        TestParallaxMapping();

        ~TestParallaxMapping() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        Camera m_Camera;
        glm::vec3 m_LightPos;
        float m_HeightScale;
        bool m_UseSteepParallax;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Shader> m_SteepShader;
        std::shared_ptr<Texture> m_DiffuseTexture;
        std::shared_ptr<Texture> m_NormalTexture;
        std::shared_ptr<Texture> m_DepthTexture;
        std::shared_ptr<VertexArray> m_VAO;
        std::shared_ptr<VertexBuffer> m_VBO;
        std::shared_ptr<IndexBuffer> m_IBO;
    };
}
