#include "version.glsl"

layout(binding = 0)uniform sampler2D uSceneTex;
layout(binding = 1)uniform sampler2D uGridTex;
layout(binding = 2)uniform sampler2D uBloomTex;
uniform vec4 uGridColor = vec4(0.6403509, 0.8421053, 1.0, 0.1019608);

#include "fbData.glsl"
in fbData o;

out vec4 outColor;

void main( void ) 
{
	outColor = texture(uSceneTex, o.texcoord);
	vec3 newColor;
	newColor = mix(outColor.rgb, uGridColor.rgb, uGridColor.a * texture(uGridTex, o.texcoord * 10).rgb);
	newColor += texture(uBloomTex, vec2(o.texcoord.x, 1 - o.texcoord.y)).rgb * 0.2;
	outColor.rgb = mix(outColor.rgb, newColor, o.color.a) * o.color.rgb;
}