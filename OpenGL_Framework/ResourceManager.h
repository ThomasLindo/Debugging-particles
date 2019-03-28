#pragma once
#include "Mesh.h"
#include "MeshDynamic.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Font.h"
#include <unordered_map>

/*
  //////////////////////
 // Resource Manager // 2019 Stephen Thompson
//////////////////////

Checks if a file has already been loaded and returns a pointer if it has,
otherwise it loads the resource and adds it to the list of resources.
The list is sorted via binary search tree, with filenames being condensed 
into a hash value to determine the order.

By far not my finest Resource Manager, interally is kind of a nightmare.
I should've just used an unordered_map, oh well...
I'll make a better version someday
*/

class ResourceManager
{
public:
	static Mesh* addMesh(const std::string &file);
	static Texture* addTexture(const std::string &file, GLenum wrapX = GL_REPEAT, GLenum wrapY = GL_REPEAT, GLenum filterMag = GL_LINEAR, GLenum filterMin = GL_LINEAR);
	static Shader* addShader(const std::string &file);
	static ShaderProgram* addShaderProgram(Shader *vertShader, Shader *fragShader);
	static ShaderProgram* addShaderProgram(Shader *vertShader, Shader *geomShader, Shader *fragShader);
	static ShaderProgram* addShaderProgramVertFrag(const std::string &vertShader, const std::string &fragShader);
	static ShaderProgram* addShaderProgramVertGeomFrag(const std::string &vertShader, const std::string &geomShader, const std::string &fragShader);
	static ShaderProgram* addShaderProgram(ShaderProgram *shader);

	static int searchList(std::vector<Resource*>& list, const std::string &file, bool &found);

	static void reloadShaders();

	//static bool initFontManager();
	//static FontManager* getFontManager();
private:
	static std::vector<Mesh*> Meshes;
	static std::vector<Texture*> Textures;
	static std::vector<Shader*> Shaders;
	static std::vector<ShaderProgram*> ShaderPrograms;
};

typedef ResourceManager rm;

