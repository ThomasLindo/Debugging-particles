#include "Shader.h"
#include "IO.h"
#include <unordered_map>
#include <assert.h>

std::unordered_map <std::string, Shader::Type> shaderFileToTypeInit()
{
	std::unordered_map <std::string, Shader::Type> m;
	m["vert"] = Shader::Type::Vertex;			//GL_VERTEX_SHADER;
	m["frag"] = Shader::Type::Fragment;			//GL_FRAGMENT_SHADER;
	m["geom"] = Shader::Type::Geometry;			//GL_GEOMETRY_SHADER;
	m["comp"] = Shader::Type::Compute;			//GL_COMPUTE_SHADER;
	m["tesc"] = Shader::Type::TessControl;		//GL_TESS_CONTROL_SHADER;
	m["tese"] = Shader::Type::TessEvaluation;	//GL_TESS_EVALUATION_SHADER;
	return m;
}

std::unordered_map <Shader::Type, Shader*> shaderTypeToDefaultInit()
{
	std::unordered_map <Shader::Type, Shader*> m;
	
	m[Shader::Type::Vertex]	= &Shader::_DefaultVert;
	m[Shader::Type::Fragment] = &Shader::_DefaultFrag;
	//m[Shader::Type::Geometry]		;
	//m[Shader::Type::Compute]		;
	//m[Shader::Type::TessControl]		;
	//m[Shader::Type::TessEvaluation] 		;

	return m;
}

static std::unordered_map <std::string, Shader::Type> shaderFileToType = shaderFileToTypeInit();
static std::unordered_map <Shader::Type, Shader*> shaderTypeToDefault = shaderTypeToDefaultInit();

bool Shader::initDefault()
{
	Shader::_DefaultFrag.load("error.frag");
	Shader::_DefaultVert.load("error.vert"); 
	Shader::_DefaultVertPass.load("error.vert");
	Shader::_DefaultFrag._DoNotUnload = true;
	Shader::_DefaultVert._DoNotUnload = true;
	Shader::_DefaultVertPass._DoNotUnload = true;

	return true;
}

Shader Shader::_DefaultFrag;
Shader Shader::_DefaultVert;
Shader Shader::_DefaultVertPass;

void Shader::setType(const std::string & filename)
{
	std::string fileType = filename.substr(filename.size() - 4);
	assert(shaderFileToType.find(fileType) != shaderFileToType.end());
	_Type = shaderFileToType.at(fileType);
}

Shader::Shader()
{

}

Shader::Shader(const std::string & filename)
{
	load(filename);
}

Shader::~Shader()
{
	unload();
}

GLuint Shader::load(const std::string &filename)
{
	SetID(filename);
	SAT_DEBUG_LOG("Loading Shader: %s", filename.c_str());

	setType(filename);
	_Filename = filename;
	
	_Shader = glCreateShader((GLenum)_Type);

	std::string source = io::readFile("../assets/shaders/" + filename);
	if (source.length() == 0)
	{
		SAT_ERROR_LOC("%s failed to load", filename.c_str());
		return 0; // No file
	}
	
	source = io::addInclude(source, "../assets/shaders/include/");
	io::printWithLineNumbers(source);

	const GLchar *temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_Shader, 1, &temp, NULL);

	glCompileShader(_Shader);

	int compileStatus;
	glGetShaderiv(_Shader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus)
	{
		SAT_DEBUG_LOG_SAFE("SUCCESS: %s", filename.c_str());
		_IsInit = true;

		return _Shader;
	}

	int logLength;
	glGetShaderiv(_Shader, GL_INFO_LOG_LENGTH, &logLength);
	SAT_ERROR_LOC("%s failed to compile", filename.c_str());
	std::string log(logLength, ' ');
	glGetShaderInfoLog(_Shader, logLength, &logLength, &log[0]);
	PRINT_LOG_WARNING("%s\n", log.c_str());
	setDefault();

	return 0;
}

GLuint Shader::reload()
{
	if (_DoNotUnload)
		return _Shader;
	unload();
	return load(_Filename);
}

GLuint Shader::setDefault()
{
	assert(shaderTypeToDefault.find(_Type) != shaderTypeToDefault.end());
	_Shader = shaderTypeToDefault[_Type]->getHandle();
	_IsInit = false;
	return _Shader;
}

Shader::Type Shader::getType()
{
	return _Type;
}

void Shader::unload()
{
	if (_DoNotUnload && _IsInit && _Shader)
	{
		glDeleteShader(_Shader);
		_Shader = 0;
	}
}
