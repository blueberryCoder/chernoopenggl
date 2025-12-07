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
    vec3 color1 = texture(u_Texture, TexCoords).rgb;

    // BT.601/BT.701 luma coefficients
    float c = dot(color1, vec3(0.299, 0.587, 0.114));

    color = vec4(vec3(c),1.0);
}
