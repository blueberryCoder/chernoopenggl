#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;


void main()
{    
   color = vec4(1.0, 0.0,0.0, 1.0);
}
