#include "version.glsl"

uniform sampler2D uSceneTex;

#include "fbData.glsl"
in fbData o;

out vec4 outColor;

void main( void ) 
{
	outColor = o.color * texture(uSceneTex, o.texcoord);
}