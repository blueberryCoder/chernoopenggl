//
// Created by blueberry on 2025/1/2.
//

#include "TestAdvancedLighting.h"

#include "../FileUtil.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test {

    TestAdvancedLighting::TestAdvancedLighting()
        : m_Camera(glm::vec3(0.0f, 0.0f, 6.0f)),
          m_LightPos(0.0f, 0.0f, 2.0f),
          m_Blinn(true) {
        SetupCursorCallback();

        m_Shader = std::make_shared<Shader>(FileUtil::shared().GetPath("./shaders/advanced_lighting.shader"));
        m_FloorTexture = std::make_shared<Texture>(FileUtil::shared().GetPath("./textures/wood.png"));

        m_VBO = std::make_shared<VertexBuffer>(m_PlaneVertices, sizeof(m_PlaneVertices));
        m_IBO = std::make_shared<IndexBuffer>(m_PlaneIndices, 6);

        VertexBufferLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO = std::make_shared<VertexArray>();
        m_VAO->AddBuffer(*m_VBO, layout);

        m_Shader->Bind();
        m_Shader->SetUniform1i("floorTexture", 0);
        m_Shader->SetUniformVec3f("lightPos", glm::vec3( m_LightPos.x, m_LightPos.y, m_LightPos.z));
        m_Shader->SetUniform1i("blinn", m_Blinn ? 1 : 0);

        m_FloorTexture->Bind(0);

        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestAdvancedLighting::~TestAdvancedLighting() {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestAdvancedLighting::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);

        glm::mat4 view = m_Camera.GetViewMatrix();
        float fov = glm::radians(m_Camera.GetZoom());
        glm::mat4 projection = glm::perspective<float>(fov, 960.0f / 540.0, 0.1f, 100.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("view", view);
        m_Shader->SetUniformMat4f("projection", projection);
        m_Shader->SetUniformVec3f("viewPos", m_Camera.GetPosition());
        m_Shader->SetUniformVec3f("lightPos", m_LightPos);
    }

    void TestAdvancedLighting::OnRender() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        Renderer renderer;

        m_Shader->Bind();
        m_Shader->SetUniform1i("blinn", m_Blinn ? 1 : 0);
        m_FloorTexture->Bind(0);

        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    void TestAdvancedLighting::OnImGuiRender() {
        ImGui::Checkbox("Blinn-Phong", &m_Blinn);
        ImGui::Text("Light: %.2f, %.2f, %.2f", m_LightPos.x, m_LightPos.y, m_LightPos.z);
    }

    void TestAdvancedLighting::ProcessInputEvent(GLFWwindow *window, float deltaTime) {
        m_Camera.ProcessInputEvent(window, deltaTime);
    }

    void TestAdvancedLighting::ProcessCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        m_Camera.ProcessCursorPosCallback(window, xpos, ypos);
    }

    void TestAdvancedLighting::ProcessMouseScroll(GLFWwindow *window, double yoffset) {
        m_Camera.ProcessMouseScroll(yoffset);
    }
}
