#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPosition = model * vec4(aPos, 1.0);
    WorldPos = worldPosition.xyz;
    gl_Position = projection * view * worldPosition;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 WorldPos;

uniform vec3 modelColor;
uniform vec3 lightDir;

void main()
{
    vec3 normal = normalize(cross(dFdx(WorldPos), dFdy(WorldPos)));
    float diffuse = max(dot(normal, normalize(-lightDir)), 0.0);
    vec3 shaded = modelColor * (0.35 + diffuse * 0.65);
    color = vec4(shaded, 1.0);
}
