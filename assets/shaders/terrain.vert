#include "version.glsl"

#include "uniform/Camera.glsl"
#include "uniform/Time.glsl"

layout(binding = 10) uniform sampler2D uTexNoise;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

#include "vData.glsl"
out vData o;

void main()
{
	o.texcoord = in_uv;
	o.texcoord.y = 1 - o.texcoord.y;
	
	mat4 uModelView = uView * uModel;
	o.norm = mat3(uModelView) * in_normal;

	// Local Space
	vec4 pos = vec4(in_vert, 1.0);
	o.pos = pos.xyz;
	// To World Space
	pos = uModel * pos;
	pos.y += texture(uTexNoise, pos.xz * 0.01).r * 8 - 4; 
	o.worldPos = pos.xyz;
	// To View Space
	pos = uView * pos;
	o.viewPos = pos.xyz;
	
	gl_Position = uProj * pos;
}
