//
// Created by blueberry on 2022/12/3.
//

#include "TestTriangle.h"

namespace test {

    TestTriangle::TestTriangle() : vb(m_Position, 3 * 2 * sizeof(float)), va(),
                                   ib(indices, 3),
                                   shader("../../res/shaders/triangle.shader")
    {

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb,layout);
        shader.Bind();

    }

    TestTriangle::~TestTriangle() {
    }

    void TestTriangle::OnUpdate(float deltaTime) {
    }

    void TestTriangle::OnRender() {
        renderer.Clear();
        shader.Bind();
        renderer.Draw(va,ib,shader);
    }

    void TestTriangle::OnImGuiRender() {
    }
}
