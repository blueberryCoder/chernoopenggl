//
// Created by blueberry on 2022/12/3.
//
#include "TestTexture2D.h"

#include "imgui/imgui.h"

namespace test {

    TestTexture2D::TestTexture2D() {
        float positions[] =
                {
                        -50.0f, -50.0f, 0.0f, 0.0f,
                        50.0f, -50.0f, 1.0f, 0.0f,
                        50.0f, 50.0f, 1.0f, 1.0f,
                        -50.0f, 50.0f, 0.0f, 1.0f,};
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_TranslationA = glm::vec3(200.0f, 200.0f, 0.0f);
        m_TranslationB = glm::vec3(400.0f, 200.0f, 0.0f);

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("../../res/shaders/Basic.shader");
        m_Texture = std::make_unique<Texture>("../../res/textures/ChernoLogo.png");
        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        m_VAO->Unbind();
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();

        m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    }

    TestTexture2D::~TestTexture2D() {

    }

    void TestTexture2D::OnUpdate(float deltaTime) {
    }

    void TestTexture2D::OnRender() {

        Renderer renderer;
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            auto mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->Bind();
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            auto mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->Bind();
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }

    void TestTexture2D::OnImGuiRender() {
        ImGui::SliderFloat3("translationA", &m_TranslationA.x, 0.0f, 960);
        ImGui::SliderFloat3("translationB", &m_TranslationB.x, 0.0f, 960);
    }
}

