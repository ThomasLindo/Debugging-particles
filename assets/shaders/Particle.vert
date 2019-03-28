#include "version.glsl"

layout(location = 0) in vec3 in_vert;	
layout(location = 1) in vec2 in_uv;		
layout(location = 2) in vec3 in_normal;	

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) out vData o;

void main()
{
	// Pass attributes through pipeline
	o.texcoord = in_uv;
	o.norm = in_normal;

	// Note: We are NOT applying the mvp transform onto the input vertex
	gl_Position = vec4(in_vert, 1.0);
}