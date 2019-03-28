#include "ResourceManager.h"
#include "IO.h"

std::vector<Mesh*> ResourceManager::Meshes;
std::vector<Texture*> ResourceManager::Textures;
std::vector<Shader*> ResourceManager::Shaders;
std::vector<ShaderProgram*> ResourceManager::ShaderPrograms;


Mesh * ResourceManager::addMesh(const std::string & file)
{
	std::vector<Resource*> resource = std::vector<Resource*>(Meshes.begin(), Meshes.end());
	bool found = false;
	int index = searchList(resource, file, found);
	if (found)
	{
		SAT_DEBUG_LOG_SAFE("Texture Already Loaded: %s", file.c_str());
		return Meshes[index];
	}
	SAT_DEBUG_LOG_WARNING("New Texture: %s", file.c_str());
	Mesh *newMesh = new Mesh();
	newMesh->loadFromObj(file);
	Meshes.insert(Meshes.begin() + index, newMesh);
	//Textures.push_back(newTexture);
	return newMesh;
}

Texture * ResourceManager::addTexture(const std::string & file, GLenum wrapX, GLenum wrapY, GLenum filterMag, GLenum filterMin)
{
	std::vector<Resource*> resource = std::vector<Resource*>(Textures.begin(), Textures.end());
	bool found = false;
	int index = searchList(resource, file, found);
	if (found)
	{
		SAT_DEBUG_LOG_SAFE("Texture Already Loaded: %s", file.c_str());
		return Textures[index];
	}
	SAT_DEBUG_LOG_WARNING("New Texture: %s", file.c_str());
	Texture *newTexture = new Texture(file, wrapX, wrapY, filterMag, filterMin);
	Textures.insert(Textures.begin() + index, newTexture);
	//Textures.push_back(newTexture);
	return newTexture;
}

Shader * ResourceManager::addShader(const std::string & file)
{
	std::vector<Resource*> resource = std::vector<Resource*>(Shaders.begin(), Shaders.end());
	bool found = false;
	int index = searchList(resource, file, found);
	if (found)
	{
		return Shaders[index];
	}
	Shader *newShader = new Shader(file);
	Shaders.insert(Shaders.begin() + index, newShader);
	//Shaders.push_back(newShader);
	return newShader;
}

ShaderProgram * ResourceManager::addShaderProgram(Shader * vertShader, Shader * fragShader)
{
	for (ShaderProgram *program : ShaderPrograms)
	{
		if (program->_VertShader->getHandle() == vertShader->getHandle() && program->_FragShader->getHandle() == fragShader->getHandle())
		{
			SAT_DEBUG_LOG_SAFE("It's a match!");
			return program;
		}
	}

	SAT_DEBUG_LOG_WARNING("It's not a match!");
	ShaderProgram *newShader = new ShaderProgram({ vertShader, fragShader });
	ShaderPrograms.push_back(newShader);
	return newShader;
}

ShaderProgram * ResourceManager::addShaderProgram(Shader * vertShader, Shader * geomShader, Shader * fragShader)
{
	for (ShaderProgram *program : ShaderPrograms)
	{
		if (program->_VertShader->getHandle() == vertShader->getHandle() && program->_GeomShader->getHandle() == geomShader->getHandle() && program->_FragShader->getHandle() == fragShader->getHandle())
		{
			SAT_DEBUG_LOG_SAFE("It's a match!");
			return program;
		}
	}

	SAT_DEBUG_LOG_WARNING("It's not a match!");
	ShaderProgram *newShader = new ShaderProgram({ vertShader, geomShader, fragShader });
	ShaderPrograms.push_back(newShader);
	return newShader;
}

ShaderProgram * ResourceManager::addShaderProgramVertFrag(const std::string &vertShader, const std::string &fragShader)
{
	Shader* vertShade = addShader(vertShader);
	Shader* fragShade = addShader(fragShader);

	return addShaderProgram(vertShade, fragShade);
}

ShaderProgram * ResourceManager::addShaderProgramVertGeomFrag(const std::string &vertShader, const std::string &geomShader, const std::string &fragShader)
{
	Shader* vertShade = addShader(vertShader);
	Shader* geomShade = addShader(geomShader);
	Shader* fragShade = addShader(fragShader);

	return addShaderProgram(vertShade, geomShade, fragShade);
}

ShaderProgram * ResourceManager::addShaderProgram(ShaderProgram * shader)
{
	for(ShaderProgram *program : ShaderPrograms)
	{	
		if (program == shader)
		{
			return program;
		}
	}
	ShaderProgram *newShader = new ShaderProgram(*shader);
	ShaderPrograms.push_back(newShader);
	return newShader;
}

int ResourceManager::searchList(std::vector<Resource*>& list, const std::string & file, bool &found)
{
	hashID id = Resource::StringToID(file);

	int min = 0;
	int max = (int)list.size() - 1;
	int mid = 0;

	//if (list.size() == 1 && list[mid]->GetID() == id)
	//{
	//	DEBUG_LOG("File %s found at index %i", file.c_str(), mid);
	//}

	if (max >= 0)
	{
		while (min <= max)
		{
			mid = min + (max - min) / 2;

			if (list[mid]->GetID() < id)
				min = mid + 1;
			else if (list[mid]->GetID() > id)
				max = mid - 1;
			else
			{
				SAT_DEBUG_LOG("File %s found at index %i", file.c_str(), mid);
				found = true;
				return mid;
			}
		}

		if (list[mid]->GetID() < id)
			++mid;
	}
	SAT_DEBUG_LOG("%i\t%s", id, file.c_str());

	found = false;
	return mid;
	//return -1;
}

void ResourceManager::reloadShaders()
{
	for (Shader* shader : Shaders)
	{
		shader->reload();
	}

	for (ShaderProgram* program : ShaderPrograms)
	{
		program->reload();
	}
}
