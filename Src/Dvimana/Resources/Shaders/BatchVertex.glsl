#version 440 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texcoord;
layout(location = 3) in float a_texindex;
layout(location = 4) in float a_tiling_factor;

out vec2    v_texcoord;
out vec4    v_color;
out float   v_texindex;
out float   v_tiling_factor;

uniform mat4 u_mvp;

void main()
{
    v_color             = a_color;
    v_texcoord          = a_texcoord;
    v_texindex          = a_texindex;
    v_tiling_factor     = a_tiling_factor;

    gl_Position         = u_mvp * vec4(a_position, 1.0);
}