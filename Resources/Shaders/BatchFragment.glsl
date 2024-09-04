#version 440 core

layout(location = 0) out vec4 color;

in vec2     v_texcoord;
in vec4     v_color;
in float    v_texindex;
in float    v_tiling_factor;

uniform sampler2D   u_textures[32];

void main()
{
    int index = int(v_texindex);
    color = texture(u_textures[index], v_texcoord * v_tiling_factor) * v_color;
}