//
// Created by blueberry on 2024/7/5.
//

#include "TestInstance.h"
#include "../VertexBufferLayout.h"
#include "../FileUtil.h"

#include "GL/glew.h"
#include "imgui/imgui.h"

#include <vector>
#include <cstddef>

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
    }

    TestInstance::~TestInstance() = default;

    void TestInstance::OnUpdate(float deltaTime) {
    }

    void TestInstance::OnRender() {
        m_Renderer.Clear();
        m_Renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void TestInstance::OnImGuiRender() {
    }
}
