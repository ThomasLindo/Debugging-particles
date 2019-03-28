#include "version.glsl"

#include "uniform/Camera.glsl"

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

#include "vData.glsl"
out vData o;

void main()
{
	o.texcoord = in_uv;

	mat4 uModelView = uView * uModel;
	o.norm = mat3(uModelView) * in_normal;
	o.pos = (uModelView * vec4(in_vert, 1.0f)).xyz;

	gl_Position = uProj * vec4(o.pos, 1.0f);
}
