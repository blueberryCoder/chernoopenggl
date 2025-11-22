//
// Created by blueberry on 2024/7/5.
//

#include "TestInstance.h"
#include "../VertexBufferLayout.h"
#include "../FileUtil.h"

namespace test {
    TestInstance::TestInstance() {
        m_Shader = std::make_unique<Shader>(FileUtil::shared().GetPath("./shaders/test_instance.shader"));
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(3);

        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        glm::vec2 translations[100];
        int index = 0;
        float offset = 0.1f;
        for (int y = -10; y < 10; y += 2) {
            for (int x = -10; x < 10; x += 2) {
                glm::vec2 translation;
                translation.x = (float) x / 10.0f + offset;
                translation.y = (float) y / 10.0f + offset;
                translations[index++] = translation;
            }
        }
        // m_Shader->Bind();
        // for (unsigned int i = 0; i < 100; i++) {
        //     m_Shader->SetUniformVec2f(("offsets[" + std::to_string(i) + "]"), translations[i]);
        // }

        m_OffsetVBO = std::make_unique<VertexBuffer>(translations, sizeof(translations));
        VertexBufferLayout offsetLayout;
        offsetLayout.Push<float>(2);
        m_VertexArray->AddBuffer(*m_OffsetVBO, offsetLayout, 2);
        GLCall(glVertexAttribDivisor(2, 1));
    }

    TestInstance::~TestInstance() = default;

    void TestInstance::OnUpdate(float deltaTime) {
    }

    void TestInstance::OnRender() {
        m_Renderer.Clear();
        m_Renderer.DrawInstanced(*m_VertexArray, *m_IndexBuffer, *m_Shader, 100);
    }

    void TestInstance::OnImGuiRender() {
    }
}
