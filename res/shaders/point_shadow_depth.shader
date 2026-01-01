#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

out vec3 FragPos;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    gl_Position = lightSpaceMatrix * worldPos;
}


#shader fragment

#version 330 core

in vec3 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos - lightPos);
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}
