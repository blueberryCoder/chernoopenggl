//
// Created by blueberry on 2025/10/7.
//

#include "TestBlending.h"

#include "../FileUtil.h"
#include "../src/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestBlending::TestBlending() {
        SetupCursorCallback();

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_blending_grass.shader"));
        m_CubeTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/marble.jpg"));
        m_FloorTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/metal.png"));
        m_TransparentTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/grass.png"),
                                                         TextureInitParams{GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE, 1});

        m_CubeVBO = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]));

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, layout);

        m_PlaneVBO = std::make_shared<VertexBuffer>(planeVertices, sizeof(planeVertices));
        m_PlaneIBO = std::make_shared<IndexBuffer>(planeIndices, sizeof(planeIndices) / sizeof(planeIndices[0]));
        m_PlaneVAO = std::make_shared<VertexArray>();
        m_PlaneVAO->AddBuffer(*m_PlaneVBO, layout);

        m_GrassVBO = std::make_shared<VertexBuffer>(transparentVertices, sizeof(transparentVertices));
        m_GrassIBO = std::make_shared<IndexBuffer>(transparentIndices,
                                                   sizeof(transparentIndices) / sizeof(transparentIndices[0]));
        m_GrassVAO = std::make_shared<VertexArray>();
        m_GrassVAO->AddBuffer(*m_GrassVBO, layout);

        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Shader->Bind();
        m_Shader->SetUniform1i("texture1", 0);

        GLCall(glEnable(GL_DEPTH_TEST));

        m_CubeTexture->Bind();
        m_FloorTexture->Bind(1);
        m_TransparentTexture->Bind(2);
    }

    TestBlending::~TestBlending() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestBlending::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestBlending::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }

    void TestBlending::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestBlending::OnImGuiRender() {
    }

    void TestBlending::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));


        Renderer renderer;
        m_Shader->Bind();

        // Draw plane.
        GLCall(glStencilMask(0x00));
        m_Shader->SetUniformMat4f("model", glm::mat4(1.0f));
        m_Shader->SetUniform1i("texture1", 1);
        renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, *m_Shader);
        // Draw 2 cube.
        m_Shader->SetUniform1i("texture1", 0);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, 0.0f, -1.0f));
        m_Shader->SetUniformMat4f("model", model);
        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

        auto model2 = glm::translate(model, glm::vec3(0.5f, 0.0f, -3.0f));
        m_Shader->SetUniformMat4f("model", model2);
        renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);


        // Draw grasses
        vector<glm::vec3> vegetation
        {
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3(1.5f, 0.0f, 0.51f),
            glm::vec3(0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3(0.5f, 0.0f, -0.6f)
        };

        std::sort(vegetation.begin(), vegetation.end(), [](glm::vec3 a, glm::vec3 b) {
            return a.z < b.z;
        });

        m_Shader->SetUniform1i("texture1", 2);

        auto grassModel = glm::mat4(1.0f);
        for (auto &tran: vegetation) {
            auto model3 = glm::translate(grassModel, tran);
            m_Shader->SetUniformMat4f("model", model3);
            renderer.Draw(*m_GrassVAO, *m_GrassIBO, *m_Shader);
        }
    }

    void TestBlending::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
        glm::mat4 view = m_Camera->GetViewMatrix();
        auto fov = glm::radians(m_Camera->GetZoom());
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 50.0);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", proj);
    }
}
