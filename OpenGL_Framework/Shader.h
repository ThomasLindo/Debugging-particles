#pragma once
#include <string>
#include <GL\glew.h>
#include "Resource.h"

class Shader : public Resource
{
public:
	enum class Type
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Compute = GL_COMPUTE_SHADER,
		TessControl = GL_TESS_CONTROL_SHADER,
		TessEvaluation = GL_TESS_EVALUATION_SHADER
	};
private:
	Type _Type;
	GLuint _Shader = 0; // Shader handle
	std::string _Filename;
	bool _IsInit = false;
	bool _DoNotUnload = false;
	void setType(const std::string &filename);

public:
	Shader();
	Shader(const std::string &filename);
	~Shader();

	// TODO: better place to put this?
	enum AttributeLocations
	{
		VERTEX = 0,
		NORMAL = 1,
		TEXCOORD0 = 2
	};

	GLuint load(const std::string &filename);
	GLuint reload();
	GLuint setDefault();
	GLuint getHandle() const { return _Shader; }
	Type getType();
	const std::string getFilename() const { return _Filename; }
	static bool initDefault();

	void unload();

	static Shader _DefaultVert;
	static Shader _DefaultVertPass;
	static Shader _DefaultFrag;
};

