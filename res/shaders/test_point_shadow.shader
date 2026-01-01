#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vs_out.FragPos = worldPos.xyz;
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = projection * view * worldPos;
}

#shader fragment

#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float far_plane;
uniform float shadowBias;
uniform int shadows;
uniform int reverse_normals;

vec3 sampleOffsetDirections[20] = vec3[](
    vec3(1.0, 1.0, 1.0), vec3(1.0, -1.0, 1.0), vec3(-1.0, -1.0, 1.0), vec3(-1.0, 1.0, 1.0),
    vec3(1.0, 1.0, -1.0), vec3(1.0, -1.0, -1.0), vec3(-1.0, -1.0, -1.0), vec3(-1.0, 1.0, -1.0),
    vec3(1.0, 1.0, 0.0), vec3(1.0, -1.0, 0.0), vec3(-1.0, -1.0, 0.0), vec3(-1.0, 1.0, 0.0),
    vec3(1.0, 0.0, 1.0), vec3(-1.0, 0.0, 1.0), vec3(1.0, 0.0, -1.0), vec3(-1.0, 0.0, -1.0),
    vec3(0.0, 1.0, 1.0), vec3(0.0, -1.0, 1.0), vec3(0.0, -1.0, -1.0), vec3(0.0, 1.0, -1.0)
);

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    int samples = 20;
    for (int i = 0; i < samples; ++i) {
        float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;
        if (currentDepth - shadowBias > closestDepth) {
            shadow += 1.0;
        }
    }
    shadow /= float(samples);
    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    if (reverse_normals == 1) {
        normal = -normal;
    }
    vec3 lightColor = vec3(0.3);
    vec3 ambient = 0.3 * lightColor;
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    float shadow = 0.0;
    if (shadows == 1) {
        shadow = ShadowCalculation(fs_in.FragPos);
    }
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    FragColor = vec4(lighting, 1.0);
}
