//
// Created by blueberry on 2022/12/4.
//

#pragma once

#include "Test.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Texture.h"

#include <memory>

namespace test {
    class TestBatchTexture : public Test {
    public:

        TestBatchTexture();
        ~TestBatchTexture() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Texture> m_Texture0;
        std::unique_ptr<Texture> m_Texture1;
        std::unique_ptr<Shader> m_Shader;
    };
}