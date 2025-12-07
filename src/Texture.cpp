//
// Created by blueberry on 2022/11/30.
//

#include "Texture.h"

#include <iostream>

#include "stb_image/stb_image.h"

Texture::Texture(const std::string &path, const TextureInitParams &params)
    : m_RendererId(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0) {
    this->m_InitParams = params;
    if (params.type == GL_TEXTURE_2D) {
        // Create 2D Texture.
        stbi_set_flip_vertically_on_load(params.flip);
        m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);
        if (m_LocalBuffer) {
            GLenum format = GL_RGBA;
            if (m_BPP == 1)
                format = GL_RED;
            else if (m_BPP == 3)
                format = GL_RGB;
            else if (m_BPP == 4)
                format = GL_RGBA;

            GLCall(glGenTextures(1, &m_RendererId));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

            // https://stackoverflow.com/questions/34497195/difference-between-format-and-internalformat
            GLCall(
                glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
            GLCall(glGenerateMipmap(GL_TEXTURE_2D));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.WRAP_S));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.WRAP_T));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            GLCall(glBindTexture(GL_TEXTURE_2D, 0));
            stbi_image_free(m_LocalBuffer);
        } else {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(m_LocalBuffer);
        }
    } else if (params.type == GL_TEXTURE_CUBE_MAP) {
        // Create CubeMap texture
        stbi_set_flip_vertically_on_load(params.flip);
        glGenTextures(1, &m_RendererId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);

        if (params.textureFaces.size() != 6) {
            std::cout << "Cubemap texture faces must be 6" << std::endl;
        }
        for (int i = 0; i < params.textureFaces.size(); i++) {
            auto data = stbi_load(params.textureFaces[i].c_str(), &m_Width, &m_Height, &m_BPP, 0);
            if (data) {
                GLenum format = GL_RGBA;
                if (m_BPP == 1)
                    format = GL_RED;
                else if (m_BPP == 3)
                    format = GL_RGB;
                else if (m_BPP == 4)
                    format = GL_RGBA;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE,
                             data);

            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            stbi_image_free(data);
        }
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }
}

Texture::Texture(const TextureInitParams& params) : m_RendererId(0),
                                                    m_FilePath(""),
                                                    m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0),
                                                    m_InitParams(params) {
    if (params.type == GL_TEXTURE_2D) {
        auto width = params.width;
        auto height = params.height;
        // Create depth texture.
        GLCall(glGenTextures(1, &m_RendererId));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        if (params.internalFormat == GL_DEPTH_COMPONENT32F) {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE));
            GLCall(glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32F,
                width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
        } else if (params.internalFormat == GL_RGB) {
             GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        }

        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        this->m_Width = width;
        this->m_Height = height;
    } else if (params.type == GL_TEXTURE_2D_MULTISAMPLE) {
        GLCall(glGenTextures(1, &m_RendererId));
        GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererId));
        GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, params.samples, GL_RGB,
                                params.width, params.height, GL_TRUE));
        GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
        this->m_Width = params.width;
        this->m_Height = params.height;
    }
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_RendererId));
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot))
    if (m_InitParams.type == GL_TEXTURE_2D) {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId))
    } else if (m_InitParams.type == GL_TEXTURE_CUBE_MAP) {
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId))
    } else if (m_InitParams.type == GL_TEXTURE_2D_MULTISAMPLE) {
        GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererId))
    }
}

void Texture::Unbind() const {
    if (m_InitParams.type == GL_TEXTURE_2D) {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0))
    } else if (m_InitParams.type == GL_TEXTURE_CUBE_MAP) {
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0))
    } else if (m_InitParams.type == GL_TEXTURE_2D_MULTISAMPLE) {
        GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0))
    }
}
