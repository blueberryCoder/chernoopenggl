//
// Created by blueberry on 2022/12/4.
//

#include "TestBatchVertex.h"
#include "../VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    TestBatchVertex::TestBatchVertex() {
        float positions[] =
                {
                        // position 2 , color 4;
                        -1.5f, -1.5f, 1.0f, 0.0f, 0.0f, 1.0f,
                        -0.5f, -1.5f, 1.0f, 0.0f, 0.0f, 1.0f,
                        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
                        -1.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,

                        0.5f, -1.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                        1.5f, -1.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                        1.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                };
        unsigned int indices[] = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4
        };

        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 6 * sizeof(float));
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6 * 2);

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
        m_Shader = std::make_unique<Shader>("../../res/shaders/batch_vertex.shader");

        auto proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", proj);
    }

    TestBatchVertex::~TestBatchVertex() {

    }

    void TestBatchVertex::OnUpdate(float deltaTime) {
    }

    void TestBatchVertex::OnRender() {
        Renderer renderer;
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void TestBatchVertex::OnImGuiRender() {
    }

}
