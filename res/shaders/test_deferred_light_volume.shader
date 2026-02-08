#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec2 screenSize;
uniform vec3 viewPos;

struct Light {
    vec3 Position;
    vec3 Color;
    float Linear;
    float Quadratic;
    float Radius;
};
uniform Light light;

void main()
{
    vec2 texCoords = gl_FragCoord.xy / screenSize;
    vec3 FragPos = texture(gPosition, texCoords).rgb;
    vec3 Normal = texture(gNormal, texCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, texCoords).rgb;
    float Specular = texture(gAlbedoSpec, texCoords).a;

    if (length(Normal) < 0.1) {
        discard;
    }

    vec3 lightDir = light.Position - FragPos;
    float distance = length(lightDir);
    if (distance > light.Radius) {
        discard;
    }
    lightDir = normalize(lightDir);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);

    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * Diffuse * light.Color;
    vec3 specular = light.Color * spec * Specular;

    float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(diffuse + specular, 1.0);
}
