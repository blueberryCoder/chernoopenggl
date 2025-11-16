#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 dummy;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = 10.0;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{    
    color = vec4(0.8, 0.1, 0.2, 1.0);
}