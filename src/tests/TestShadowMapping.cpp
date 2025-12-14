//
// Created by blueberry on 2025/1/2.
//

#include "TestShadowMapping.h"
#include "../FileUtil.h"
#include "../WindowManager.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {
    TestShadowMapping::TestShadowMapping()
        : m_LightPos(-2.0f, 4.0f, -1.0f) {
        SetupCursorCallback();
        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 3.0f, 6.0f));

        m_ShadowMapTexture = std::make_shared<Texture>(TextureInitParams{
            .width = SHADOW_WIDTH,
            .height = SHADOW_HEIGHT,
            .internalFormat = GL_DEPTH_COMPONENT,
        });
        m_ShadowMapFramebuffer = std::make_shared<FrameBuffer>();
        m_ShadowMapFramebuffer->AttachDepth(m_ShadowMapTexture);
        m_ShadowMapFramebuffer->Bind();
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        m_ShadowMapFramebuffer->Unbind();

        m_DepthShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/simple_depth.shader"));
        m_DebugShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/debug_quad_depth.shader"));
        m_SceneShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_shadowmapping.shader"));
        m_SceneShader->Bind();
        m_SceneShader->SetUniform1i("diffuseTexture", 0);
        m_SceneShader->SetUniform1i("shadowMap", 1);
        m_SceneShader->Unbind();

        m_PlaneVBO = std::make_shared<VertexBuffer>(m_PlaneVertices, sizeof(m_PlaneVertices));
        m_PlaneIBO = std::make_shared<IndexBuffer>(m_PlaneIndices, 6);
        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_PlaneVAO = std::make_shared<VertexArray>();
        m_PlaneVAO->AddBuffer(*m_PlaneVBO, layout);

        m_QuadVBO = std::make_shared<VertexBuffer>(m_QuadVertices, sizeof(m_QuadVertices));
        m_QuadIBO = std::make_shared<IndexBuffer>(m_QuadIndices, 6);
        VertexBufferLayout quadLayout{};
        quadLayout.Push<float>(3);
        quadLayout.Push<float>(2);
        m_QuadVAO = std::make_shared<VertexArray>();
        m_QuadVAO->AddBuffer(*m_QuadVBO, quadLayout);

        m_CubeVBO = std::make_shared<VertexBuffer>(m_CubeVertices, sizeof(m_CubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(m_CubeIndices, 36);
        VertexBufferLayout cubeLayout{};
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(2);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);

        m_DiffuseTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/wood.png"));

        GLCall(glEnable(GL_DEPTH_TEST));
    };

    TestShadowMapping::~TestShadowMapping() {
        GLCall(glDisable(GL_DEPTH_TEST));
    };

    void TestShadowMapping::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
    }

    void TestShadowMapping::OnRender() {
        Renderer renderer;

        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
        glm::mat4 lightView = glm::lookAt(m_LightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // Depth pass
        GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
        m_ShadowMapFramebuffer->Bind();
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));

        m_DepthShader->Bind();
        m_DepthShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

        auto renderScene = [&](Shader &shader) {
            glm::mat4 model = glm::mat4(1.0f);
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, shader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
            model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
            model = glm::scale(model, glm::vec3(0.25f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);
        };

        renderScene(*m_DepthShader);

        m_ShadowMapFramebuffer->Unbind();

        int viewportWidth = 960;
        int viewportHeight = 540;
        WindowManager::shared().GetViewportSize(viewportWidth, viewportHeight);
        if (viewportWidth <= 0 || viewportHeight <= 0) {
            viewportWidth = 960;
            viewportHeight = 540;
        }

        GLCall(glViewport(0, 0, viewportWidth, viewportHeight));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        if (m_ShowDebugDepth) {
            m_DebugShader->Bind();
            m_DebugShader->SetUniform1i("depthMap", 0);
            m_DebugShader->SetUniform1f("near_plane", 1.0f);
            m_DebugShader->SetUniform1f("far_plane", 20.0f);
            m_ShadowMapTexture->Bind(0);

            renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_DebugShader);
            return;
        }

        glm::mat4 view = m_Camera->GetViewMatrix();
        float fov = glm::radians(m_Camera->GetZoom());
        float aspect = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
        glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 25.0f);

        m_SceneShader->Bind();
        m_SceneShader->SetUniformMat4f("projection", projection);
        m_SceneShader->SetUniformMat4f("view", view);
        m_SceneShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
        m_SceneShader->SetUniformVec3f("lightPos", m_LightPos);
        m_SceneShader->SetUniformVec3f("viewPos", m_Camera->GetPosition());

        m_DiffuseTexture->Bind(0);
        m_ShadowMapTexture->Bind(1);

        renderScene(*m_SceneShader);
    }

    void TestShadowMapping::OnImGuiRender() {
        ImGui::Checkbox("Show depth debug", &m_ShowDebugDepth);
    }

    void TestShadowMapping::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestShadowMapping::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestShadowMapping::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }
}
