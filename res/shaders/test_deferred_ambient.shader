#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gAlbedoSpec;
uniform float ambientStrength;

void main()
{
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 ambient = Diffuse * ambientStrength;
    FragColor = vec4(ambient, 1.0);
}
