//
// Created by blueberry on 2022/12/4.
//

#include "TestBatchDynamic.h"
#include "../VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image/stb_image.h"

namespace test {

    TestBatchDynamic::TestBatchDynamic() {

        GLCall(glGenVertexArrays(1, &m_VA0))
        GLCall(glBindVertexArray(m_VA0))

        GLCall(glGenBuffers(1, &m_VertexBuffer))
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer))
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW));

        GLCall(glGenBuffers(1, &m_IndexBuffer))
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer))
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 1000 * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW))

        GLCall(glEnableVertexAttribArray(0))
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (const void *) offsetof(Vertex, position)))

        GLCall(glEnableVertexAttribArray(1))
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (const void *) offsetof(Vertex, texCord)))

        GLCall(glEnableVertexAttribArray(2))
        GLCall(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                     (const void *) offsetof(Vertex, texIndex)))

        m_Shader = std::make_unique<Shader>("../../res/shaders/batch_texture.shader");

        unsigned int textures[2];
        GLCall(glGenTextures(2,textures));
        LoadTexture(textures[0],"../../res/textures/gray_cat.png");
        LoadTexture(textures[1],"../../res/textures/orange_cat.png");

        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D,textures[0]));

        GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_2D,textures[1]));

        auto proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", proj);
        int array[2] = {0, 1};
        m_Shader->SetUniform1iv("u_Textures", 2, array);

    }

    TestBatchDynamic::~TestBatchDynamic() {

    }

    void TestBatchDynamic::OnUpdate(float deltaTime) {
//        float positions[] =
//                {
//                        // position 2 , texture 2  index 1;
//                        -1.5f, -1.5f, 0.0f, 0.0f, 0.0f,
//                        -0.5f, -1.5f, 1.0f, 0.0f, 0.0f,
//                        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
//                        -1.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//
//                        0.5f, -1.5f, 0.0f, 0.0f, 1.0f,
//                        1.5f, -1.5f, 1.0f, 0.0f, 1.0f,
//                        1.5f, -0.5f, 1.0f, 1.0f, 1.0f,
//                        0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
//                };
//
//        unsigned int indices[] = {
//                0, 1, 2, 2, 3, 0,
//                4, 5, 6, 6, 7, 4
//        };
    }

    void TestBatchDynamic::OnRender() {

        int offset = 0;
        std::array<Vertex, 4 * 25> vertexs{};

        float iX = -1.5f, iY = -1.5f;
        for (int i = 0; i < 5; i++) {
            iX += 1.5f;
            iY = -1.5f;
            for (int j = 0; j < 5; ++j) {
                iY += 1.5f;
                auto array = CreateQuad(iX, iY, i % 2 == 0 ? 1.0f : 0.0f);
                std::copy(array.begin(), array.end(), vertexs.begin() + offset );
                offset += 4;
            }
        }
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0,vertexs.size() * sizeof (Vertex), vertexs.data()));

        std::array<unsigned int , 25 * 6> indices {} ;
        for (int i = 0; i < 25; i++) {
            indices[6 * i + 0] = 0 + i * 4;
            indices[6 * i + 1] = 1 + i * 4;
            indices[6 * i + 2] = 2 + i * 4;
            indices[6 * i + 3] = 2 + i * 4;
            indices[6 * i + 4] = 3 + i * 4;
            indices[6 * i + 5] = 0 + i * 4;
        }

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data()));

        GLCall(glBindVertexArray(m_VA0));
        m_Shader->Bind();

        GLCall(glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT, nullptr));
    }

    void TestBatchDynamic::OnImGuiRender() {
    }


    std::array<TestBatchDynamic::Vertex, 4> TestBatchDynamic::CreateQuad(float x, float y, float index) {
        Vertex v1{
                x, y, 0.0f,
                0.0f, 0.0f,
                index
        };

        Vertex v2{
                x + 1.0f, y, 0.0f,
                1.0f, 0.0f,
                index
        };

        Vertex v3{
                x + 1.0f, y + 1.0f, 0.0f,
                1.0f, 1.0f,
                index
        };

        Vertex v4{
                x, y + 1.0f, 0.0f,
                0.0f, 1.0f,
                index
        };

        return {v1, v2, v3, v4};
    }

    void TestBatchDynamic::LoadTexture(unsigned int &textureId,const std::string &textureRes) {
        GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        stbi_set_flip_vertically_on_load(1);
        int width = 0, height = 0, channel = 0;
        auto localBuffer = stbi_load(textureRes.c_str(), &width, &height, &channel, 4);
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
        if (localBuffer) {
            stbi_image_free(localBuffer);
        }
    }

}
