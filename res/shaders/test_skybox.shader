#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 aTexCoords;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = gl_Position.xyww;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 TexCoords;
uniform samplerCube texture1;

void main()
{    
    color = texture(texture1, TexCoords);
}