#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

out vec2    v_Texcoord;
out vec4    v_Color;
out float   v_TexIndex;
out float   v_TilingFactor;

uniform mat4 u_MVP;

void main()
{
    v_Color             = a_Color;
    v_Texcoord          = a_Texcoord;
    v_TexIndex          = a_TexIndex;
    v_TilingFactor     = a_TilingFactor;

    gl_Position         = u_MVP * vec4(a_Position, 1.0);
}