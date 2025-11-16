#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 TexCoords;
uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{    
   if (gl_FrontFacing)
        color = texture(frontTexture, TexCoords);
   else
        color = texture(backTexture, TexCoords);
}
