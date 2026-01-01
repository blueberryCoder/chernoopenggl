//
// Created by blueberry on 2025/11/15.
//

#include "TestPointShadow.h"
#include "../FileUtil.h"
#include "../WindowManager.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <array>

namespace test {
    TestPointShadow::TestPointShadow()
        : m_LightPos(0.0f, 4.0f, 0.0f) {
        SetupCursorCallback();
        m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 3.0f, 8.0f));

        m_ShadowMapTexture = std::make_shared<Texture>(TextureInitParams{
            .WRAP_S = GL_CLAMP_TO_EDGE,
            .WRAP_T = GL_CLAMP_TO_EDGE,
            .type = GL_TEXTURE_CUBE_MAP,
            .width = SHADOW_WIDTH,
            .height = SHADOW_HEIGHT,
            .internalFormat = GL_DEPTH_COMPONENT
        });
        m_ShadowMapFramebuffer = std::make_shared<FrameBuffer>();
        m_ShadowMapFramebuffer->AttachDepth(m_ShadowMapTexture);
        m_ShadowMapFramebuffer->Bind();
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        m_ShadowMapFramebuffer->Unbind();

        m_DepthShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/point_shadow_depth.shader"));
        m_LightShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/light_point.shader"));
        m_SceneShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_point_shadow.shader"));
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

        m_CubeVBO = std::make_shared<VertexBuffer>(m_CubeVertices, sizeof(m_CubeVertices));
        m_CubeIBO = std::make_shared<IndexBuffer>(m_CubeIndices, 36);
        VertexBufferLayout cubeLayout{};
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(2);
        m_CubeVAO = std::make_shared<VertexArray>();
        m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);

        m_LightVBO = std::make_shared<VertexBuffer>(m_LightVertices, sizeof(m_LightVertices));
        m_LightIBO = std::make_shared<IndexBuffer>(m_LightIndices, 1);
        VertexBufferLayout lightLayout{};
        lightLayout.Push<float>(3);
        m_LightVAO = std::make_shared<VertexArray>();
        m_LightVAO->AddBuffer(*m_LightVBO, lightLayout);

        m_DiffuseTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/wood.png"));

        GLCall(glEnable(GL_DEPTH_TEST));
    };

    TestPointShadow::~TestPointShadow() {
        GLCall(glDisable(GL_DEPTH_TEST));
        GLCall(glDisable(GL_CULL_FACE));
        GLCall(glPointSize(1.0f));

    };

    void TestPointShadow::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
    }

    void TestPointShadow::OnRender() {
        Renderer renderer;

        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, m_LightNear, m_LightFar);
        std::array<glm::mat4, 6> shadowTransforms = {
            shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        };

        GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
        m_ShadowMapFramebuffer->Bind();
        m_DepthShader->Bind();
        m_DepthShader->SetUniformVec3f("lightPos", m_LightPos);
        m_DepthShader->SetUniform1f("far_plane", m_LightFar);

        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));

        auto renderScene = [&](Shader &shader, bool supportReverseNormals) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(5.0f));
            shader.SetUniformMat4f("model", model);
            GLboolean cullEnabled = GL_FALSE;
            GLCall(cullEnabled = glIsEnabled(GL_CULL_FACE));
            if (cullEnabled == GL_TRUE) {
                GLCall(glDisable(GL_CULL_FACE));
            }
            if (supportReverseNormals) {
                shader.SetUniform1i("reverse_normals", 1);
            }
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);
            if (supportReverseNormals) {
                shader.SetUniform1i("reverse_normals", 0);
            }
            if (cullEnabled == GL_TRUE) {
                GLCall(glEnable(GL_CULL_FACE));
            }

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.75f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5f));
            model = glm::scale(model, glm::vec3(0.5f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0f));
            model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
            model = glm::scale(model, glm::vec3(0.75f));
            shader.SetUniformMat4f("model", model);
            renderer.Draw(*m_CubeVAO, *m_CubeIBO, shader);
        };

        for (size_t i = 0; i < shadowTransforms.size(); ++i) {
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(i), m_ShadowMapTexture->GetID(), 0));
            GLCall(glClear(GL_DEPTH_BUFFER_BIT));
            m_DepthShader->SetUniformMat4f("lightSpaceMatrix", shadowTransforms[i]);
            renderScene(*m_DepthShader, false);
        }

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

        glm::mat4 view = m_Camera->GetViewMatrix();
        float fov = glm::radians(m_Camera->GetZoom());
        float aspect = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
        glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

        m_SceneShader->Bind();
        m_SceneShader->SetUniformMat4f("projection", projection);
        m_SceneShader->SetUniformMat4f("view", view);
        m_SceneShader->SetUniformVec3f("lightPos", m_LightPos);
        m_SceneShader->SetUniformVec3f("viewPos", m_Camera->GetPosition());
        m_SceneShader->SetUniform1f("far_plane", m_LightFar);
        m_SceneShader->SetUniform1f("shadowBias", m_ShadowBias);
        m_SceneShader->SetUniform1i("shadows", m_EnableShadows ? 1 : 0);

        m_DiffuseTexture->Bind(0);
        m_ShadowMapTexture->Bind(1);

        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_BACK));
        renderScene(*m_SceneShader, true);

        m_LightShader->Bind();
        m_LightShader->SetUniformMat4f("projection", projection);
        m_LightShader->SetUniformMat4f("view", view);
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), m_LightPos);
        m_LightShader->SetUniformMat4f("model", lightModel);
        GLCall(glPointSize(8.0f));
        renderer.DrawPoints(*m_LightVAO, *m_LightIBO, *m_LightShader);
    }

    void TestPointShadow::OnImGuiRender() {
        ImGui::Checkbox("Enable shadow", &m_EnableShadows);
        ImGui::SliderFloat("Shadow Bias", &m_ShadowBias, 0.0f, 0.2f);
        ImGui::DragFloat3("Light Pos", &m_LightPos.x, 0.1f);
        ImGui::SliderFloat("Light Near", &m_LightNear, 0.1f, 10.0f);
        ImGui::SliderFloat("Light Far", &m_LightFar, 10.0f, 120.0f);
        glm::vec3 camPos = m_Camera->GetPosition();
        if (ImGui::DragFloat3("Camera Pos", &camPos.x, 0.1f)) {
            m_Camera->SetPosition(camPos);
        }
        if (m_LightFar <= m_LightNear + 0.1f) {
            m_LightFar = m_LightNear + 0.1f;
        }
    }

    void TestPointShadow::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera->ProcessInputEvent(window, deltaTime);
    }

    void TestPointShadow::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera->ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestPointShadow::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera->ProcessMouseScroll(yoffset);
    }
}
