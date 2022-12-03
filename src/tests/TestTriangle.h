//
// Created by blueberry on 2022/12/3.
//

#pragma once

#include "Test.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

namespace test {

    class TestTriangle : public Test {

    public:
        TestTriangle();

        ~TestTriangle();

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

    private:
        float m_Position [3][2] {
           -0.5,-0.5f,
           0.5f,-0.5f,
           0.0f,0.5f,
        };
        unsigned  int indices[3]  = {0,1,2};

        VertexArray va;
        VertexBuffer vb;
        IndexBuffer ib;
        Shader shader;
        Renderer renderer;
    };
}
