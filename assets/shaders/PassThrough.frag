#include "version.glsl"

uniform sampler2D uSceneTex;

#include "fbData.glsl"
in fbData o;

out vec4 outColor;

void main( void ) 
{
	outColor = texture(uSceneTex, o.texcoord);
}