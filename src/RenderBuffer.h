//
// Created by blueberry on 2025/10/26.
//

#pragma once
#include <GL/glew.h>

struct RenderBufferInitParams {
    int width = 0;
    int height = 0;
    int format = GL_DEPTH24_STENCIL8;
};

class RenderBuffer {
public:
    explicit RenderBuffer(const RenderBufferInitParams &params);

    ~RenderBuffer();

    void Bind() const;

    void Unbind() const;

    unsigned int GetID() const {
        return m_RendererID;
    }

private:
    unsigned int m_RendererID{};
};
