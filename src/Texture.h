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
    int flip = 1;
    // InitParams() : WRAP_S(GL_REPEAT), WRAP_T(GL_REPEAT), flip(1) {}
};

class Texture {
public:

private:
    unsigned int m_RendererId;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width,m_Height,m_BPP;
    std::string m_Type;

public :
    explicit Texture(const std::string &path, const TextureInitParams& params  = TextureInitParams());

    Texture(int width, int height, int format = GL_DEPTH_COMPONENT);

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
