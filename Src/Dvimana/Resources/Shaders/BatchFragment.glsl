#version 440 core

layout(location = 0) out vec4 FragColor;

in vec2     v_Texcoord;
in vec4     v_Color;
in float    v_TexIndex;
in float    v_TilingFactor;

uniform sampler2D   u_Textures[32];

void main()
{
    int index = int(v_TexIndex);
    FragColor = texture(u_Textures[index], v_Texcoord * v_TilingFactor) * v_Color;
}