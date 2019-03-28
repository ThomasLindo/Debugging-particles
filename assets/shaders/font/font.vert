#version 420
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 texcoord;

#include "uniform/Camera.glsl"

void main()
{
    gl_Position = uProj * vec4(vertex.xy, 0.0, 1.0);
    texcoord = vertex.zw;
}  