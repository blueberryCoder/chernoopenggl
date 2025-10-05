//
// Created by blueberry on 2022/11/30.
//

#ifndef MYOPENGL_TEXTURE_H
#define MYOPENGL_TEXTURE_H

#include "Renderer.h"
using namespace std;
class Texture {

private:
    unsigned int m_RendererId;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width,m_Height,m_BPP;
    std::string m_Type;

public :
    Texture(const std::string &path, int flip = 1);

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
