#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aModel;

out vec2 TexCoords;

// uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * aModel * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Material {
    sampler2D texture_diffuse1;

    sampler2D texture_specular1;
};

in vec2 TexCoords;

uniform Material material;

void main()
{    
    color = texture(material.texture_diffuse1, TexCoords);

}