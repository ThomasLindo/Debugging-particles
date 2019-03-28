#include "version.glsl"

layout(binding = 0) uniform sampler2D uTexColor;

uniform vec4 uColor = vec4(1.0);

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) in vData o;

out vec4 outColor;

float rand(vec2 co)
{
    return fract(sin(1000.0*dot(co.xy, vec2(21.5739,43.421))) * 617284.3);
}

void main()
{
	vec2 uv = o.texcoord.xy;
	vec4 source = texture(uTexColor, uv) * uColor;

	outColor = source;
}