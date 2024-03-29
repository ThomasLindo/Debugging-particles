#include "version.glsl"

layout(location = 0)	in vec3 in_vert;	
layout(location = 1)	in vec2 in_uv;		

#include "fbData.glsl"
out fbData o;

void main()
{
	o.texcoord = in_uv;
	gl_Position = vec4(in_vert, 1.0f); //Set up the vertex data as a vec4 in view space so we can use it.
}