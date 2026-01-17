#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);

    T = normalize(T - dot(T, N) * N);
    B = normalize(B - dot(B, N) * N);

    vs_out.TBN = mat3(T, B, N);
    vs_out.FragPos = fragPos;
    vs_out.TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(fragPos, 1.0);
}

#shader fragment

#version 330 core
layout(location = 0) out vec4 color;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int blinn;

void main()
{
    vec3 albedo = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 normal = texture(material.texture_normal1, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.TBN * normal);

    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    if (blinn == 1) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    vec3 specularMap = texture(material.texture_specular1, fs_in.TexCoords).rgb;
    vec3 ambient = 0.1 * albedo;
    vec3 diffuse = diff * albedo;
    vec3 specular = spec * specularMap;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}
