#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * position;
   v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

float near = 0.1;
float far  = 50.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    // http://www.songho.ca/opengl/gl_projectionmatrix.html
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
   float depth = texture(u_Texture, v_TexCoord).r;
   depth = LinearizeDepth(depth) / far;
   color = vec4(vec3(depth), 1.0);
}