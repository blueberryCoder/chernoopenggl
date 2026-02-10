//
// Created by blueberry on 2026/2/10.
//

#pragma once

#include "Test.h"
#include "../Camera.h"
#include "../FrameBuffer.h"
#include "../IndexBuffer.h"
#include "../Model.h"
#include "../RenderBuffer.h"
#include "../Renderer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "glm/glm.hpp"

#include <memory>
#include <random>
#include <vector>

namespace test {
    class TestSSAO : public Test {
    public:
        TestSSAO();

        ~TestSSAO() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:
        void CreateGBuffer(int width, int height);

        void BuildKernel();

        void CreateNoiseTexture();

        bool m_WasDepthEnabled = false;
        bool m_WasBlendEnabled = false;
        int m_GBufferWidth = 0;
        int m_GBufferHeight = 0;

        std::shared_ptr<Camera> m_Camera;
        std::shared_ptr<Model> m_Model;
        std::shared_ptr<Shader> m_GeometryShader;
        std::shared_ptr<Shader> m_SSAOShader;

        std::shared_ptr<FrameBuffer> m_GBuffer;
        std::shared_ptr<Texture> m_GPosition;
        std::shared_ptr<Texture> m_GNormal;
        std::shared_ptr<Texture> m_GAlbedo;
        std::shared_ptr<RenderBuffer> m_DepthRbo;

        std::shared_ptr<VertexArray> m_QuadVAO;
        std::shared_ptr<VertexBuffer> m_QuadVBO;
        std::shared_ptr<IndexBuffer> m_QuadIBO;

        Renderer m_Renderer;

        std::vector<glm::vec3> m_SSAOKernel;
        std::mt19937 m_Rng;
        unsigned int m_NoiseTexture = 0;

        glm::mat4 m_View{1.0f};
        glm::mat4 m_Projection{1.0f};

        float m_QuadVertices[20] = {
            // positions   // texcoords
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
        };
        unsigned int m_QuadIndices[6] = {0, 1, 2, 0, 2, 3};
    };
}
