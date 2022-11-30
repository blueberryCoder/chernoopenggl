//
// Created by blueberry on 2022/11/29.
//

#ifndef MYOPENGL_VERTEXARRAY_H
#define MYOPENGL_VERTEXARRAY_H
#include "VertexBuffer.h"
class VertexBufferLayout;
class VertexArray {
private :
    unsigned int m_RendererID;
public :
    VertexArray();

    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};

#endif //MYOPENGL_VERTEXARRAY_H
