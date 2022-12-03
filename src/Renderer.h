//
// Created by blueberry on 2022/11/29.
//

#ifndef MYOPENGL_RENDERER_H
#define MYOPENGL_RENDERER_H

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x))  __builtin_trap();

#define GLCall(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall(#x, __FILE_NAME__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);


class Renderer {
public :
    void Clear() const ;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
};

#endif //MYOPENGL_RENDERER_H
