//
// Created by blueberry on 2025/10/26.
//

#include "RenderBuffer.h"

#include "Renderer.h"

RenderBuffer::RenderBuffer(const RenderBufferInitParams &params) {
    GLCall(glGenRenderbuffers(1, &m_RendererID));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
    if (params.samples > 1) {
        GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, params.samples,
            params.format, params.width, params.height));
    } else {
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, params.format, params.width, params.height));
    }
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

RenderBuffer::~RenderBuffer() {
    GLCall(glDeleteRenderbuffers(1, &m_RendererID));
}

void RenderBuffer::Bind() const {
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::Unbind() const {
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}
