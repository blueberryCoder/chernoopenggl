//
// Created by blueberry on 2025/10/27.
//

#pragma once

#include <GL/glew.h>
#include <memory>

#include "../Camera.h"
#include "../IndexBuffer.h"
#include "../Renderer.h"
#include "../Shader.h"
#include "../RenderBuffer.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "Test.h"
#include "../FrameBuffer.h"

namespace test {
    class TestAntiAliasingEffect : public Test {
    public:
        TestAntiAliasingEffect();

        ~TestAntiAliasingEffect() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        std::shared_ptr<Shader> m_Shader, m_EffectShader;
        std::shared_ptr<VertexArray> m_VAO;
        std::shared_ptr<VertexBuffer> m_VBO;
        std::shared_ptr<IndexBuffer> m_IBO;

        std::shared_ptr<Camera> m_Camera;

        std::shared_ptr<FrameBuffer> m_FBO;
        std::shared_ptr<Texture> m_FboTexture;
        std::shared_ptr<RenderBuffer> m_DepthRbo;

        std::shared_ptr<VertexArray> m_EffectVAO;
        std::shared_ptr<VertexBuffer> m_EffectVBO;
        std::shared_ptr<IndexBuffer> m_EffectIBO;

        Renderer m_Renderer;

        std::shared_ptr<Texture> m_IntermediateTexture;
        std::shared_ptr<FrameBuffer> m_IntermediateFBO;

        float m_Rotation = 0.0f;
        float m_RotationSpeed = 0.8f;

        GLboolean m_WasDepthEnabled = GL_FALSE;
        GLboolean m_WasMultisampleEnabled = GL_FALSE;
    };
}
