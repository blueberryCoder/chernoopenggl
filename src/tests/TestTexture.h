//
// Created by blueberry on 2022/12/3.
//

#pragma once

#include "Test.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../Renderer.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include "../Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

    class TestTexture : public Test {
    public:
        TestTexture();

        ~TestTexture();

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

    private:
        float m_Position[4][4];
        unsigned int m_Indices[6];
        VertexArray va;
        VertexBuffer vb;
        IndexBuffer ib;
        Shader shader;
        Texture texture;
        Renderer renderer;
        glm::mat4 mvp;
        glm::vec3 translationA;
        glm::vec3 translationB;


    };
}