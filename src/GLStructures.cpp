//
// Created by blueberry on 2025/10/3.
//

#include "GLStructures.h"

#include <iostream>
#include <ostream>

#include "VertexBufferLayout.h"

// namespace data {
    Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<shared_ptr<Texture>> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    void Mesh::setupMesh() {
        m_VertexArray = std::make_shared<VertexArray>();
        m_IBO = std::make_shared<IndexBuffer>(indices.data(), indices.size());
        m_VBO = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));

        VertexBufferLayout layout = {};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VertexArray->AddBuffer(*m_VBO, layout);
    }

    void Mesh::Draw(Shader &shader) {
        shader.Bind();
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size() && i< 3; i++) {
            auto texture = textures[i];
            texture->Bind(i);
            string number;
            string name = texture->GetType();
            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            } else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            }
            // std::cout <<"Shader set material." << name << number << ":" << i << std::endl;
            shader.SetUniform1i("material." + name + number, i);
        }

        // draw mesh
        m_VertexArray->Bind();
        m_Renderer.Draw(*m_VertexArray, *m_IBO, shader);
        m_VertexArray->Unbind();

    }
// }
