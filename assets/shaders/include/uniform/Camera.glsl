layout(std140, binding = 1) uniform Camera
{
	uniform mat4 uProj;
	uniform mat4 uView;
};
uniform mat4 uModel;