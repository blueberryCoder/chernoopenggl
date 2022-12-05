//
// Created by blueberry on 2022/12/4.
//

#include "TestBatchTexture.h"
#include "../VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    TestBatchTexture::TestBatchTexture() {
        float positions[] =
                {
                        // position 2 , texture 2  index 1;
                        -1.5f, -1.5f, 0.0f, 0.0f, 0.0f,
                        -0.5f, -1.5f, 1.0f, 0.0f, 0.0f,
                        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
                        -1.5f, -0.5f, 0.0f, 1.0f, 0.0f,

                        0.5f, -1.5f, 0.0f, 0.0f, 1.0f,
                        1.5f, -1.5f, 1.0f, 0.0f, 1.0f,
                        1.5f, -0.5f, 1.0f, 1.0f, 1.0f,
                        0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
                };

        unsigned int indices[] = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4
        };

        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 5 * sizeof(float));
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6 * 2);

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
        m_Shader = std::make_unique<Shader>("../../res/shaders/batch_texture.shader");

        m_Texture0 = std::make_unique<Texture>( "../../res/textures/gray_cat.png");
        m_Texture1 = std::make_unique<Texture>( "../../res/textures/orange_cat.png");

        auto proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);

        m_Texture0->Bind(0);
        m_Texture1->Bind(1);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", proj);
        int array[2] = {0, 1};
        m_Shader->SetUniform1iv("u_Textures", 2, array);
    }

    TestBatchTexture::~TestBatchTexture() {

    }

    void TestBatchTexture::OnUpdate(float deltaTime) {
    }

    void TestBatchTexture::OnRender() {
        Renderer renderer;
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void TestBatchTexture::OnImGuiRender() {
    }

}
