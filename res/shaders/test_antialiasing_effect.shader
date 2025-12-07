#shader vertex

#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;


void main()
{
    TexCoords = aTexCoords;    
    gl_Position = vec4(aPos,0.0, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 TexCoords;
uniform sampler2D u_Texture;

void main()
{    
    color = texture(u_Texture, TexCoords);
}