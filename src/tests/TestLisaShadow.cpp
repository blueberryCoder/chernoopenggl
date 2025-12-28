//
// Created by blueberry on 2025/1/2.
//

#include "TestLisaShadow.h"
#include "../FileUtil.h"
#include "../WindowManager.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {
    TestLisaShadow::TestLisaShadow()
        : m_LightPos(-4.0f, 8.0f, -4.0f) {
        SetupCursorCallback();
        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 2.0f, 8.0f));

        m_ShadowMapTexture = std::make_shared<Texture>(TextureInitParams{
            .WRAP_S = GL_CLAMP_TO_EDGE,
            .WRAP_T = GL_CLAMP_TO_EDGE,
            .width = SHADOW_WIDTH,
            .height = SHADOW_HEIGHT,
            .internalFormat = GL_DEPTH_COMPONENT,
            .borderColor = std::make_shared<glm::vec4>(glm::vec4(1.0, 1.0, 1.0, 1.0))
        });
        m_ShadowMapFramebuffer = std::make_shared<FrameBuffer>();
        m_ShadowMapFramebuffer->AttachDepth(m_ShadowMapTexture);
        m_ShadowMapFramebuffer->Bind();
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        m_ShadowMapFramebuffer->Unbind();

        m_DepthShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/simple_depth.shader"));
        m_DebugShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/debug_quad_depth.shader"));
        m_SceneShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_lisa_shadow.shader"));
        m_SceneShader->Bind();
        m_SceneShader->SetUniform1i("material.texture_diffuse1", 0);
        m_SceneShader->SetUniform1i("material.texture_specular1", 0);
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

        m_PlaneTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/wood.png"));
        m_LisaModel = std::make_shared<Model>(FileUtil::shared().GetPath("./models/lisa/lisa.obj"));

        GLCall(glEnable(GL_DEPTH_TEST));
    };

    TestLisaShadow::~TestLisaShadow() {
        GLCall(glDisable(GL_DEPTH_TEST));
        GLCall(glDisable(GL_CULL_FACE));
    };

    void TestLisaShadow::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
    }

    void TestLisaShadow::OnRender() {
        Renderer renderer;

        glm::mat4 lightProjection = glm::ortho(-m_LightOrthoSize, m_LightOrthoSize, -m_LightOrthoSize,
            m_LightOrthoSize, m_LightNear, m_LightFar);
        glm::mat4 lightView = glm::lookAt(m_LightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // Depth pass
        GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
        m_ShadowMapFramebuffer->Bind();
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));

        m_DepthShader->Bind();
        m_DepthShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));

        glm::mat4 model = glm::mat4(1.0f);
        m_DepthShader->SetUniformMat4f("model", model);
        renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, *m_DepthShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, m_ModelPosition);
        model = glm::scale(model, glm::vec3(m_ModelScale));
        m_DepthShader->SetUniformMat4f("model", model);
        m_LisaModel->Draw(*m_DepthShader);

        GLCall(glCullFace(GL_BACK));
        GLCall(glDisable(GL_CULL_FACE));

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
            m_ShadowMapTexture->Bind(0);

            renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_DebugShader);
            return;
        }

        glm::mat4 view = m_Camera->GetViewMatrix();
        float fov = glm::radians(m_Camera->GetZoom());
        float aspect = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
        glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

        m_SceneShader->Bind();
        m_SceneShader->SetUniformMat4f("projection", projection);
        m_SceneShader->SetUniformMat4f("view", view);
        m_SceneShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
        m_SceneShader->SetUniformVec3f("lightPos", m_LightPos);
        m_SceneShader->SetUniformVec3f("viewPos", m_Camera->GetPosition());

        m_PlaneTexture->Bind(0);
        m_ShadowMapTexture->Bind(1);

        model = glm::mat4(1.0f);
        m_SceneShader->SetUniformMat4f("model", model);
        m_SceneShader->SetUniform1i("material.texture_diffuse1", 0);
        m_SceneShader->SetUniform1i("material.texture_specular1", 0);
        renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, *m_SceneShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, m_ModelPosition);
        model = glm::scale(model, glm::vec3(m_ModelScale));
        m_SceneShader->SetUniformMat4f("model", model);
        m_LisaModel->Draw(*m_SceneShader);
    }

    void TestLisaShadow::OnImGuiRender() {
        ImGui::Checkbox("Show depth debug", &m_ShowDebugDepth);
        ImGui::DragFloat3("Light Pos", &m_LightPos.x, 0.1f);
        ImGui::SliderFloat("Light Ortho Size", &m_LightOrthoSize, 5.0f, 50.0f);
        ImGui::SliderFloat("Light Near", &m_LightNear, 0.1f, 5.0f);
        ImGui::SliderFloat("Light Far", &m_LightFar, 10.0f, 120.0f);
        ImGui::DragFloat3("Model Pos", &m_ModelPosition.x, 0.05f);
        ImGui::SliderFloat("Model Scale", &m_ModelScale, 0.05f, 2.0f);
        if (m_LightFar <= m_LightNear + 0.1f) {
            m_LightFar = m_LightNear + 0.1f;
        }
    }

    void TestLisaShadow::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestLisaShadow::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestLisaShadow::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }
}
