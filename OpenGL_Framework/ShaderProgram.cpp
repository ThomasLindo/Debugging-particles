#include "ShaderProgram.h"
#include <iostream>
#include <gmtk/gmtk.h>
#include <fstream>
#include "IO.h"
#include <assert.h>
#include "ResourceManager.h"

bool ShaderProgram::_IsInitDefault = false;
ShaderProgram ShaderProgram::_ProgramDefault = ShaderProgram();
GLuint ShaderProgram::_CurrentlyBoundProgram = 0;

ShaderProgram::ShaderProgram()
{

}

ShaderProgram::ShaderProgram(const std::string & vertFile, const std::string & fragFile)
{
	load(vertFile, fragFile);
}

ShaderProgram::ShaderProgram(const std::vector<Shader*> &shaders)
{
	load(shaders);
}

ShaderProgram::~ShaderProgram()
{
	if (_IsInit)
	{
		unload();
	}
}

bool ShaderProgram::initDefault()
{
	if (!_IsInitDefault)
	{
		if (!Shader::initDefault())
		{
			SAT_DEBUG_LOG_ERROR("Could not compile default Shaders!");
		}

		_ProgramDefault.attachShader(&Shader::_DefaultVert);
		_ProgramDefault.attachShader(&Shader::_DefaultFrag);
		int compileSuccess = _ProgramDefault.linkProgram();
		if (!compileSuccess)
		{
			SAT_DEBUG_LOG_ERROR("Could not link default Shaders!");
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
		_IsInitDefault = true;
	}
	return _IsInitDefault;
}

void ShaderProgram::setDefault()
{

}

void ShaderProgram::attachShader(Shader * shader)
{
	if (!_Program)
	{
		_Program = glCreateProgram();
	}

	if (shader->getHandle())
	{
		glAttachShader(_Program, shader->getHandle());
		switch (shader->getType())
		{
		case Shader::Type::Vertex:
			_VertShader = shader;
			_AttachedShaders.push_back(shader);
			break;
		case Shader::Type::Fragment:
			_FragShader = shader;
			_AttachedShaders.push_back(shader);
			break;
		case Shader::Type::Geometry:
			_GeomShader = shader;
			_AttachedShaders.push_back(shader);
			break;
		case Shader::Type::Compute:
			_CompShader = shader;
			_AttachedShaders.push_back(shader);
			break;
		case Shader::Type::TessControl:
			_TescShader = shader;
			_AttachedShaders.push_back(shader);
			break;
		case Shader::Type::TessEvaluation:
			_TeseShader = shader;
			_AttachedShaders.push_back(shader);
			break;
		default:
			SAT_ERROR_LOC("UNKNOWN SHADER TYPE: %d\n%s", shader->getHandle(), shader->getFilename().c_str());
		}

	}
	else
	{
		SAT_ERROR_LOC("UHOH");
	}
}

bool ShaderProgram::load(const std::string & vertFile, const std::string & fragFile)
{
	_AttachedShaders.clear();
	_VertShader = ResourceManager::addShader(vertFile);
	_FragShader = ResourceManager::addShader(fragFile);
	attachShader(_VertShader);
	attachShader(_FragShader);
	_AttachedShaders.push_back(_VertShader);
	_AttachedShaders.push_back(_FragShader);
	vertFilename = vertFile;
	fragFilename = fragFile;
	return linkProgram();
}

bool ShaderProgram::load(const std::vector<Shader*> &shaders)
{
	_AttachedShaders.clear();
	for (size_t i = 0; i < shaders.size(); i++)
	{
		attachShader(shaders[i]);
	}
	return linkProgram();
}

bool ShaderProgram::load(const std::vector<std::string>& fileNames)
{

	return linkProgram();
}

bool ShaderProgram::reload()
{
	unload();

	std::vector<Shader*> temp = _AttachedShaders;
	return load(temp);
}

bool ShaderProgram::isLoaded() const
{
	return _IsInit;
}

void ShaderProgram::unload()
{
	if (_Program) // If the shader exists, remove it.
	{
		for (size_t i = 0; i < _AttachedShaders.size(); i++)
		{
			glDetachShader(_Program, _AttachedShaders[i]->getHandle());
		}

		glDeleteProgram(_Program);
		_Program = 0;
	}
	_IsInit = false;
}

GLuint ShaderProgram::linkProgram()
{
	assert(_Program);

	glLinkProgram(_Program);
	GLint success;
	glGetProgramiv(_Program, GL_LINK_STATUS, &success);
	if (success)
	{
		_IsInit = true;
		return _Program;
	}
	else
	{
		int logLength;
		glGetProgramiv(_Program, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, ' ');

		SAT_DEBUG_LOG_ERROR("Shader Program failed to compile");
		glGetProgramInfoLog(_Program, logLength, &logLength, &log[0]);
		SAT_DEBUG_LOG_WARNING("%s", log.c_str());

		return false;
	}
}

void ShaderProgram::bind() const
{
	glUseProgram(_Program);
#if _DEBUG || DEBUG_LOG
	_CurrentlyBoundProgram = _Program;
#endif
}

void ShaderProgram::unbind()
{
	glUseProgram(GL_NONE);
#if _DEBUG || DEBUG_LOG
	_CurrentlyBoundProgram = GL_NONE;
#endif
}


GLint ShaderProgram::getUniformLocationSafe(const std::string & uniformName) const
{
	return GLint();
}

GLint ShaderProgram::getUniformLocation(const std::string & uniformName) const
{
#if _DEBUG || DEBUG_LOG
	if (_Program != _CurrentlyBoundProgram)
	{
		SAT_DEBUG_LOG_WARNING("WARNING: Uniform %s : Shader is not currently bound!", uniformName.c_str());
	}
#endif
	GLint uniformLoc = glGetUniformLocation(_Program, uniformName.c_str());
#if _DEBUG || DEBUG_LOG
	if (uniformLoc == -1)
	{
		SAT_DEBUG_LOG_WARNING("WARNING: Uniform %s not found in Program %u!", uniformName.c_str(), _Program);
	}
#endif
	return uniformLoc;
}

void ShaderProgram::sendUniform(const std::string & name, float scalar) const
{
	GLint location = getUniformLocation(name);
	glUniform1f(location, scalar);
}

void ShaderProgram::sendUniform(const std::string & name, const vec3 & vector) const
{
	GLint location = getUniformLocation(name);
	glUniform3fv(location, 1, &vector.x);
}

void ShaderProgram::sendUniform(const std::string & name, const vec4 & vector) const
{
	GLint location = getUniformLocation(name);
	glUniform4fv(location, 1, &vector.x);
}

void ShaderProgram::sendUniform(const std::string & name, const mat4 & matrix, bool transpose) const
{
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix.arr);
}

bool ShaderProgram::compileShader(GLuint shader) const
{
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	return success == GL_TRUE;
}

void ShaderProgram::outputShaderLog(GLuint shader) const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
	glGetShaderInfoLog(shader, sizeof(char) * 512, &infoLen, &infoLog[0]); //Size of char array in bits for portability, rather than characters.
	std::cout << std::string(infoLog.begin(), infoLog.end()) << "\n";
}

void ShaderProgram::outputProgramLog() const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetProgramiv(_Program, GL_INFO_LOG_LENGTH, &infoLen);
	glGetProgramInfoLog(_Program, sizeof(char) * 512, &infoLen, &infoLog[0]); // Size of char array in bits for portability, rather than characters.
	std::cout << std::string(infoLog.begin(), infoLog.end()) << "\n";
}
