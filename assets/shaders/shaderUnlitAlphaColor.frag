#include "version.glsl"

uniform vec4 uColor = vec4(1,1,1,1);

layout(binding = 0) uniform sampler2D uTex;

#include "vData.glsl"
in vData o;

out vec4 outColor;

void main()
{
	float alpha = texture(uTex, o.texcoord).r;
	outColor = uColor;
	outColor.a *= alpha;
}
