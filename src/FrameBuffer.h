//
// Created by blueberry on 2025/10/12.
//

#pragma once
#include "Texture.h"

class FrameBuffer {
public:
    FrameBuffer();

    virtual ~FrameBuffer();

    void Bind();
    void Unbind();

    void AttachDepth(const std::shared_ptr<Texture>& texture);

    unsigned int GetID() { return m_RendererID;}

private:
    unsigned int m_RendererID;
};
