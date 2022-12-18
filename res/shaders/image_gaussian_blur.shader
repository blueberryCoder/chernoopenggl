#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
    gl_Position = position;
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location =0) out vec4 color;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

uniform int u_BlurRadius;
uniform vec2 u_BlurOffset;

const float PI = 3.1415926;
uniform float u_SumWeight;

vec2 clampCoordinate(vec2 corrdinate) {
    return vec2(clamp(corrdinate.x, 0.0, 1.0), clamp(corrdinate.y, 0.0, 1.0));
}

float getWeight(int i) {
    float sigma = 5.0;
    return (1.0 / sqrt(2.0 * PI * sigma * sigma)) * exp(-float(i*i) / (2.0 * sigma * sigma)) / u_SumWeight ;
}

void main() {
    vec4 sourceColor = texture(u_Texture, v_TexCoord);

    if (u_BlurRadius <= 1.0)  {
        color = sourceColor;
        return;
    }
    float weight = getWeight(0);

    vec3 finalColor = sourceColor.rgb * weight;

    for (int i=1; i < u_BlurRadius; i++) {
        weight = getWeight(i);
        finalColor += texture(u_Texture, clampCoordinate(v_TexCoord + u_BlurOffset * float(i))).rgb * weight;
        finalColor += texture(u_Texture, clampCoordinate(v_TexCoord - u_BlurOffset * float(i))).rgb * weight;
    }

    color = vec4(finalColor, sourceColor.a);
}