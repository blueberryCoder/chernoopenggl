//
// Created by blueberry on 2025/11/16.
//

#include "UniformBuffer.h"

#include "FrameBuffer.h"

UniformBuffer::UniformBuffer(const void * data, unsigned int size) {
    GLCall(glGenBuffers(1,&m_RendererID));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER,m_RendererID));
    GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW));
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER,0));
}

UniformBuffer::~UniformBuffer() {
    GLCall(glDeleteBuffers(1,&m_RendererID));
}

void UniformBuffer::Bind() {
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER,m_RendererID));
}

void UniformBuffer::Unbind() {
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER,0));
}

void UniformBuffer::UpdateData(unsigned int offset, unsigned int size, const void *data) {
    Bind();
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER,offset,size,data));
}

void UniformBuffer::BindRangeToPoint(unsigned int point, unsigned int offset, unsigned int size) {
    Bind();
    GLCall(glBindBufferRange(GL_UNIFORM_BUFFER,point,m_RendererID,offset,size));
}
