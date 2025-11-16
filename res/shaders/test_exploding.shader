#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 normal;
    vec2 texCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.normal = aNormal;
    vs_out.texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader geometry
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time;

in VS_OUT {
    vec3 normal;
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords;

vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction,0.0);
}

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main()
{
     vec3 normal = GetNormal();
     gl_Position = explode(gl_in[0].gl_Position, normal);
     TexCoords = gs_in[0].texCoords;
     EmitVertex();
     gl_Position = explode(gl_in[1].gl_Position, normal);
     TexCoords = gs_in[1].texCoords;
     EmitVertex();
     gl_Position = explode(gl_in[2].gl_Position, normal);
     TexCoords = gs_in[2].texCoords;
     EmitVertex();
    EndPrimitive();
}

#shader fragment
#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

layout(location = 0) out vec4 color;

in vec2 TexCoords;
uniform Material material;

void main()
{
    color = texture(material.texture_diffuse1, TexCoords);
}
