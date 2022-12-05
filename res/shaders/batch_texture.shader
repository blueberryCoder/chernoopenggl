#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float textureIndex;

out vec2 v_TexCoord;
out float v_TextureIndex;
uniform mat4 u_MVP;
void main()
{
   gl_Position =  u_MVP * position ;
   v_TexCoord = texCoord;
   v_TextureIndex = textureIndex;
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;
in float v_TextureIndex;
in vec2 v_TexCoord;
uniform sampler2D u_Textures[2];

void main()
{
   int index = int(v_TextureIndex);
   color = texture(u_Textures[index], v_TexCoord);
//   color = vec4(1.0,v_TextureIndex,v_TextureIndex,1.0);
}