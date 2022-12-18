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

vec2 clampCoordinate(vec2 corrdinate) {
    return vec2(clamp(corrdinate.x, 0.0, 1.0), clamp(corrdinate.y, 0.0, 1.0));
}

void main() {
    vec4 sourceColor = texture(u_Texture, v_TexCoord);

    if (u_BlurRadius <= 1.0)  {
        color = sourceColor;
        return;
    }
    vec3 finalColor = sourceColor.rgb;
    for (int i = 1; i < u_BlurRadius; i++) {
        finalColor += texture(u_Texture, clampCoordinate(v_TexCoord + u_BlurOffset * float(i))).rgb;
        finalColor += texture(u_Texture, clampCoordinate(v_TexCoord - u_BlurOffset * float(i))).rgb;
    }
    finalColor /= float(2 * u_BlurRadius + 1);

    color = vec4(finalColor, sourceColor.a);
}