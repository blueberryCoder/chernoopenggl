#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vColor = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec3 vColor;
layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
