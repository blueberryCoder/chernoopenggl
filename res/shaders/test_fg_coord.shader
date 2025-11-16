#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{    
   if (gl_FragCoord.x < 600)
        color = vec4(1.0, 0.0, 0.0, 1.0);
   else
        color = vec4(0.0, 1.0, 0.0, 1.0);
}