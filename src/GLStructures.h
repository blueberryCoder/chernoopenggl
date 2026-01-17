//
// Created by blueberry on 2025/10/3.
//

#pragma once

#include <string>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "glm/glm.hpp"
#include  "Shader.h"
#include  "Texture.h"

// namespace data {

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

class Mesh {

public:
    // mesh data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<shared_ptr<Texture>>      textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<shared_ptr<Texture>> textures);
    void Draw(Shader &shader, unsigned int count = 0);

    std::shared_ptr<VertexArray> GetVertexArray();
private:

    // render data
    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_IBO;
    Renderer m_Renderer;

    void setupMesh();
};

// }
