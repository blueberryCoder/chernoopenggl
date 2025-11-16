//
// Created by blueberry on 2025/11/16.
//

#include "TestAdvanced.h"

#include "../FileUtil.h"
#include  "../src/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestAdvanced::TestAdvanced() : m_Camera(glm::vec3(0.0f, 0.0f, 1.0f)) {
        GLCall(glPointSize(20.0f));

        m_PointShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_point_size.shader"));
        m_FacingShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_fg_facing.shader"));
        m_FgCoordsShader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_fg_coord.shader"));

        m_FrontTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/front_face.png"));
        m_BackTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/back_face.png"));

        m_PointVBO = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
        m_PointIBO = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(cubeIndices[0]));

        VertexBufferLayout layout = {};
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_PointVAO = std::make_shared<VertexArray>();
        m_PointVAO->AddBuffer(*m_PointVBO, layout);

        SetupCursorCallback();
        m_PointShader->Bind();
        m_PointShader->SetUniformMat4f("view", glm::mat4(1.0f));
        m_PointShader->SetUniformMat4f("projection", glm::mat4(1.0f));
        m_PointShader->SetUniformMat4f("model", glm::mat4(1.0f));

        m_FacingShader->Bind();
        m_FacingShader->SetUniformMat4f("view", glm::mat4(1.0f));
        m_FacingShader->SetUniformMat4f("projection", glm::mat4(1.0f));
        m_FacingShader->SetUniformMat4f("model", glm::mat4(1.0f));
        m_FacingShader->SetUniform1i("frontTexture", 0);
        m_FacingShader->SetUniform1i("backTexture", 1);

        m_FgCoordsShader->Bind();
        m_FgCoordsShader->SetUniformMat4f("view", glm::mat4(1.0f));
        m_FgCoordsShader->SetUniformMat4f("projection", glm::mat4(1.0f));
        m_FgCoordsShader->SetUniformMat4f("model", glm::mat4(1.0f));


        m_UBO = std::make_shared<UniformBuffer>(nullptr, sizeof(glm::mat4) *2);
        // bind point 0;
        m_UBO->Bind();
        m_UBO->BindRangeToPoint(0, 0, 2 * sizeof(glm::mat4));

        const std::array<std::string, 4> uboShaderFiles = {
            "./shaders/test_ubo_red.shader",
            "./shaders/test_ubo_green.shader",
            "./shaders/test_ubo_blue.shader",
            "./shaders/test_ubo_yellow.shader"
        };
        for (size_t i = 0; i < uboShaderFiles.size(); ++i) {
            auto shader = std::make_shared<Shader>(FileUtil::shared().GetPath(uboShaderFiles[i]));
            shader->BindUniformToPoint("Matrices", 0);
            m_UboShaders[i] = shader;
        }

    };

    TestAdvanced::~TestAdvanced() {
        GLCall(glPointSize(1.0f));
        GLCall(glDisable(GL_DEPTH_TEST));

    };

    void TestAdvanced::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        glm::mat4 view = m_Camera.GetViewMatrix();
        auto fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 proj = glm::perspective(fov, 960.0 / 540.0, 0.1, 100.0);
        m_PointShader->Bind();
        m_PointShader->SetUniformMat4f("view", view);
        m_PointShader->SetUniformMat4f("projection", proj);

        m_FacingShader->Bind();
        m_FacingShader->SetUniformMat4f("view", view);
        m_FacingShader->SetUniformMat4f("projection", proj);

        m_FgCoordsShader->Bind();
        m_FgCoordsShader->SetUniformMat4f("view", view);
        m_FgCoordsShader->SetUniformMat4f("projection", proj);

        m_UBO->UpdateData(0, sizeof(glm::mat4), &proj[0][0]);
        m_UBO->UpdateData(sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
    };


    void TestAdvanced::OnRender() {
        Renderer renderer;
        renderer.Clear();
        {
            m_PointShader->Bind();
            m_PointShader->SetUniformMat4f("model", glm::mat4(1.0f));
            renderer.DrawPoints(*m_PointVAO, *m_PointIBO, *m_PointShader);
        }

        {
            m_FgCoordsShader->Bind();
            m_FgCoordsShader->SetUniformMat4f("model",
                                              glm::translate(glm::mat4(1.0f), glm::vec3(2.0, 1.0, 1.0)));
            renderer.Draw(*m_PointVAO, *m_PointIBO, *m_FgCoordsShader);
        }

        {
            GLCall(glEnable(GL_DEPTH_TEST));

            m_FacingShader->Bind();
            m_FacingShader->SetUniformMat4f("model",
                                            glm::translate(glm::mat4(1.0f), glm::vec3(-2.0, 1.0, 1.0)));
            m_FrontTexture->Bind(0);
            m_BackTexture->Bind(1);
            renderer.Draw(*m_PointVAO, *m_PointIBO, *m_FacingShader);
            GLCall(glDisable(GL_DEPTH_TEST));

        }

        {
            const std::array<glm::vec3, 4> offsets = {
                glm::vec3(-4.0f, -1.0f, 0.0f),
                glm::vec3(-2.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(2.0f, -1.0f, 0.0f)
            };
            for (size_t i = 0; i < m_UboShaders.size(); ++i) {
                auto &shader = m_UboShaders[i];
                if (!shader) {
                    continue;
                }
                shader->Bind();
                shader->SetUniformMat4f("model", glm::translate(glm::mat4(1.0f), offsets[i]));
                shader->BindUniformToPoint("Matrices", 0);
                renderer.Draw(*m_PointVAO, *m_PointIBO, *shader);
            }
        }
    };

    void TestAdvanced::OnImGuiRender() {
    };

    void TestAdvanced::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestAdvanced::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestAdvanced::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}
