#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include <vector>

/*
  ////////////////
 // GameObject // 2019 Stephen Thompson
////////////////

The GameObject class is used for easily representing objects in the scene,
each object gets a reference to a mesh, multiple textures, and a material,
which determines what shader the object is drawn with

*/

class GameObject : public Transform
{
	
public: 
	GameObject();
	GameObject(const std::string &meshFile, const std::string &textureFile);
	GameObject(const std::string &meshFile, const std::vector <std::string> &textureFiles); 
	GameObject(Mesh* _mesh, Texture *_texture);
	GameObject(Mesh* _mesh, std::vector <Texture*>& _textures);
	~GameObject();

	void setMesh(Mesh* _mesh);
	void setTexture(Texture* _texture);
	void setTextureBinds(std::vector<TextureBind> _textures);
	void setTextures(std::vector<Texture*>& _textures);
	void draw();

protected:
	Mesh* mesh;
	std::vector<TextureBind> textures;
};