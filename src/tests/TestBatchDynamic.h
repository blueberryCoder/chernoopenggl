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

#include <array>
#include <memory>

namespace test {
    class TestBatchDynamic : public Test {
        struct Position {
            float x, y, z;
        };

        struct TexCord {
            float x, y;
        };

        struct Vertex {
            Position position;
            TexCord texCord;
            float texIndex;
        };

    public:

        TestBatchDynamic();

        ~TestBatchDynamic() override;

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

    private:
        unsigned int m_VA0;
        unsigned int m_VertexBuffer;
        unsigned int m_IndexBuffer;
        unsigned int m_Textures[2];

        std::unique_ptr<Shader> m_Shader;

        static void LoadTexture(unsigned int &textureId, const std::string &textureRes);

        static std::array<TestBatchDynamic::Vertex, 4> CreateQuad(float x, float y, float texIndex);
    };

}