//
// Created by blueberry on 2026/3/21.
//

#include "TestPBR.h"

#include "../FileUtil.h"
#include "../VertexBufferLayout.h"
#include "../WindowManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include <cmath>

namespace {
    constexpr float kPi = 3.14159265359f;
}

namespace test {
    TestPBR::TestPBR()
        : m_Camera(glm::vec3(0.0f, 0.0f, 20.0f)) {
        SetupCursorCallback();
        m_WasDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLCall(glEnable(GL_DEPTH_TEST));

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/test_pbr.shader"));

        BuildSphereMesh();
        m_SphereVBO = std::make_shared<VertexBuffer>(m_SphereVertices.data(),
                                                     static_cast<unsigned int>(m_SphereVertices.size() * sizeof(float)));
        m_SphereIBO = std::make_shared<IndexBuffer>(m_SphereIndices.data(),
                                                    static_cast<unsigned int>(m_SphereIndices.size()));

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_SphereVAO = std::make_shared<VertexArray>();
        m_SphereVAO->AddBuffer(*m_SphereVBO, layout);

        m_Shader->Bind();
        m_Shader->SetUniformVec3f("albedo", m_Albedo);
        m_Shader->SetUniform1f("ao", m_Ao);
    }

    TestPBR::~TestPBR() {
        if (!m_WasDepthEnabled) {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
    }

    void TestPBR::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        int fbWidth = 0;
        int fbHeight = 0;
        WindowManager::shared().GetFramebufferSize(fbWidth, fbHeight);
        float aspect = (fbHeight != 0)
            ? static_cast<float>(fbWidth) / static_cast<float>(fbHeight)
            : (960.0f / 540.0f);

        glm::mat4 view = m_Camera.GetViewMatrix();
        float fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", projection);
        m_Shader->SetUniformVec3f("camPos", m_Camera.GetPosition());
        m_Shader->SetUniformVec3f("albedo", m_Albedo);
        m_Shader->SetUniform1f("ao", m_Ao);

        for (int i = 0; i < 4; ++i) {
            m_Shader->SetUniformVec3f("lightPositions[" + std::to_string(i) + "]", m_LightPositions[i]);
            m_Shader->SetUniformVec3f("lightColors[" + std::to_string(i) + "]", m_LightColors[i]);
        }
    }

    void TestPBR::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_Shader->Bind();
        for (int row = 0; row < m_RowCount; ++row) {
            float metallic = static_cast<float>(row) / static_cast<float>(std::max(1, m_RowCount - 1));
            m_Shader->SetUniform1f("metallic", metallic);

            for (int col = 0; col < m_ColumnCount; ++col) {
                float roughness = static_cast<float>(col) / static_cast<float>(std::max(1, m_ColumnCount - 1));
                roughness = std::max(0.05f, std::min(roughness, 1.0f));

                glm::mat4 model(1.0f);
                model = glm::translate(
                    model,
                    glm::vec3(
                        (static_cast<float>(col) - static_cast<float>(m_ColumnCount) / 2.0f) * m_Spacing,
                        (static_cast<float>(row) - static_cast<float>(m_RowCount) / 2.0f) * m_Spacing,
                        0.0f
                    )
                );
                model = glm::scale(model, glm::vec3(m_SphereScale));
                glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

                m_Shader->SetUniformMat4f("model", model);
                m_Shader->SetUniformMat3f("normalMatrix", normalMatrix);
                m_Shader->SetUniform1f("roughness", roughness);
                m_Renderer.Draw(*m_SphereVAO, *m_SphereIBO, *m_Shader);
            }
        }
    }

    void TestPBR::OnImGuiRender() {
        ImGui::ColorEdit3("Albedo", &m_Albedo[0]);
        ImGui::SliderFloat("AO", &m_Ao, 0.0f, 1.0f);
        ImGui::SliderFloat("Spacing", &m_Spacing, 1.2f, 4.0f);
        ImGui::SliderFloat("Sphere Scale", &m_SphereScale, 0.2f, 1.0f);
        ImGui::Text("Rows: metallic, Columns: roughness");
    }

    void TestPBR::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestPBR::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestPBR::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }

    void TestPBR::BuildSphereMesh() {
        const unsigned int xSegments = 64;
        const unsigned int ySegments = 64;

        m_SphereVertices.clear();
        m_SphereIndices.clear();
        m_SphereVertices.reserve((xSegments + 1) * (ySegments + 1) * 8);
        m_SphereIndices.reserve(xSegments * ySegments * 6);

        for (unsigned int y = 0; y <= ySegments; ++y) {
            for (unsigned int x = 0; x <= xSegments; ++x) {
                float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
                float ySegment = static_cast<float>(y) / static_cast<float>(ySegments);
                float xPos = std::cos(xSegment * 2.0f * kPi) * std::sin(ySegment * kPi);
                float yPos = std::cos(ySegment * kPi);
                float zPos = std::sin(xSegment * 2.0f * kPi) * std::sin(ySegment * kPi);

                m_SphereVertices.push_back(xPos);
                m_SphereVertices.push_back(yPos);
                m_SphereVertices.push_back(zPos); // Position
                m_SphereVertices.push_back(xPos);
                m_SphereVertices.push_back(yPos);
                m_SphereVertices.push_back(zPos); // Normal
                m_SphereVertices.push_back(xSegment);
                m_SphereVertices.push_back(ySegment); // UV
            }
        }

        for (unsigned int y = 0; y < ySegments; ++y) {
            for (unsigned int x = 0; x < xSegments; ++x) {
                unsigned int i0 = y * (xSegments + 1) + x;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = (y + 1) * (xSegments + 1) + x;
                unsigned int i3 = i2 + 1;

                m_SphereIndices.push_back(i0);
                m_SphereIndices.push_back(i2);
                m_SphereIndices.push_back(i1);
                m_SphereIndices.push_back(i1);
                m_SphereIndices.push_back(i2);
                m_SphereIndices.push_back(i3);
            }
        }
    }
}
