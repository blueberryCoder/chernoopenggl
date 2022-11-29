//
// Created by blueberry on 2022/11/29.
//

#include <GL/glew.h>


#ifndef MYOPENGL_RENDERER_H
#define MYOPENGL_RENDERER_H

#define ASSERT(x) if (!(x))  __builtin_trap();

#define GLCall(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall(#x, __FILE_NAME__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);


#endif //MYOPENGL_RENDERER_H
