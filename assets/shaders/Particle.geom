#include "version.glsl"

#include "uniform/Camera.glsl"

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float uSize = 1.0;

struct vData
{
	vec2 texcoord;
	vec3 norm;
	vec3 pos;
};
layout(location = 0) out vData o;

// Creates a quad of specified size around point p
void PointToQuadBillboarded(vec4 p, float size)
{
	float halfSize = size * 0.5;

	vec4 pEye = uView * uModel * p;

	//Shift the points
	vec4 topLeft =		vec4(pEye.xy+vec2(-halfSize, halfSize), pEye.z, 1.0);
	vec4 topRight =		vec4(pEye.xy+vec2(halfSize, halfSize), pEye.z, 1.0);
	vec4 bottomLeft =	vec4(pEye.xy+vec2(-halfSize, -halfSize), pEye.z, 1.0);
	vec4 bottomRight =	vec4(pEye.xy+vec2(halfSize, -halfSize), pEye.z, 1.0);

	gl_Position = uProj * bottomLeft;
	o.texcoord.xy = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = uProj * bottomRight;
	o.texcoord.xy = vec2(1.0, 1.0);
	EmitVertex();

	gl_Position = uProj * topLeft;
	o.texcoord.xy = vec2(0.0, 0.0);
	EmitVertex();

	gl_Position = uProj * topRight;
	o.texcoord.xy = vec2(1.0, 0.0);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	PointToQuadBillboarded(gl_in[0].gl_Position, uSize);
}