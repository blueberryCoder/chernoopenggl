//
// Created by blueberry on 2025/10/12.
//

#include "FrameBuffer.h"

#include "Renderer.h"

FrameBuffer::FrameBuffer():m_RendererID(0) {
    GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer() {
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::AttachDepth(const std::shared_ptr<Texture>& texture) {
    Bind();
    GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->GetID(), 0));
}

void FrameBuffer::AttachDepthStencil(const std::shared_ptr<RenderBuffer>& rbo) {
    Bind();
    rbo->Bind();
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->GetID()););
}

void FrameBuffer::Bind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Unbind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}




