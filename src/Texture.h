//
// Created by blueberry on 2022/11/30.
//

#ifndef MYOPENGL_TEXTURE_H
#define MYOPENGL_TEXTURE_H

#include "Renderer.h"
using namespace std;

struct TextureInitParams {
    int WRAP_S = GL_REPEAT;
    int WRAP_T = GL_REPEAT;
    int MIN_FILTER = GL_NEAREST;
    int MAG_FILTER = GL_NEAREST;
    int flip = 1;
    int type = GL_TEXTURE_2D;

    int samples = 4;

    // for cube map
    std::vector<std::string> textureFaces;

    int width;
    int height;

    unsigned int internalFormat = GL_RGB;

    std::shared_ptr<glm::vec4> borderColor;
};

class Texture {

private:
    unsigned int m_RendererId;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width,m_Height,m_BPP;
    std::string m_Type;

    TextureInitParams m_InitParams;

public :
    explicit Texture(const std::string &path, const TextureInitParams& params  = TextureInitParams());

    Texture(const TextureInitParams& params  = TextureInitParams());

    ~Texture();

    void Bind(unsigned int slot = 0) const;

    void Unbind() const;

    inline int GetWidth() const { return m_Width; }

    inline int GetHeight() const { return m_Height; }

    string GetType() {
        return m_Type;
    }
    void SetType(std::string type) {
        this->m_Type = type;
    }

    unsigned int GetID() {
        return m_RendererId;
    }

    string GetPath() {
        return m_FilePath;
    }
};


#endif //MYOPENGL_TEXTURE_H
