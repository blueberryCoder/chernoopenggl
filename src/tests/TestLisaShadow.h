//
// Created by blueberry on 2025/1/2.
//

#pragma once

#include "Test.h"
#include "../Camera.h"
#include "../FrameBuffer.h"
#include "../IndexBuffer.h"
#include "../Model.h"
#include "../Renderer.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "glm/glm.hpp"
#include <memory>

namespace test {

    class TestLisaShadow : public Test {
    public:
        TestLisaShadow();

        ~TestLisaShadow() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

        void ProcessInputEvent(GLFWwindow *window, float deltaTime) override;

        void ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) override;

        void ProcessMouseScroll(GLFWwindow *window, double yoffset) override;

    private:

        const int SHADOW_WIDTH = 1024;
        const int SHADOW_HEIGHT = 1024;
        bool m_ShowDebugDepth = false;
        float m_LightOrthoSize = 25.0f;
        float m_LightNear = 0.5f;
        float m_LightFar = 60.0f;
        float m_ModelScale = 0.25f;
        glm::vec3 m_ModelPosition = glm::vec3(0.0f);

        std::shared_ptr<Camera> m_Camera;
        std::shared_ptr<Model> m_LisaModel;
        std::shared_ptr<Texture> m_PlaneTexture;
        std::shared_ptr<Texture> m_ShadowMapTexture;
        std::shared_ptr<FrameBuffer> m_ShadowMapFramebuffer;
        std::shared_ptr<Shader> m_DepthShader;
        std::shared_ptr<Shader> m_DebugShader;
        std::shared_ptr<Shader> m_SceneShader;
        std::shared_ptr<VertexArray> m_PlaneVAO;
        std::shared_ptr<VertexBuffer> m_PlaneVBO;
        std::shared_ptr<IndexBuffer> m_PlaneIBO;
        std::shared_ptr<VertexArray> m_QuadVAO;
        std::shared_ptr<VertexBuffer> m_QuadVBO;
        std::shared_ptr<IndexBuffer> m_QuadIBO;

        glm::vec3 m_LightPos;

        float m_PlaneVertices[48] = {
            // positions            // normals         // texcoords
            25.0f,  -0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
            -25.0f, -0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -25.0f, -0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

            25.0f,  -0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
            -25.0f, -0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
            25.0f,  -0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f
        };

        unsigned int m_PlaneIndices[6] = {0, 1, 2, 3, 4, 5};

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
