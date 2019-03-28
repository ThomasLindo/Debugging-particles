#include "GameObject.h"
#include <stdio.h>

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string & meshFile, const std::string & textureFile)
{
	printf("not done yet");
}

GameObject::GameObject(const std::string &meshFile, const std::vector<std::string> &textureFiles)
{
	printf("not done yet");
}

GameObject::GameObject(Mesh * _mesh, Texture * _texture)
{
	setMesh(_mesh);
	setTexture(_texture);
}

GameObject::GameObject(Mesh * _mesh, std::vector<Texture*> &_textures)
{
	setMesh(_mesh);
	setTextures(_textures);
}

GameObject::~GameObject()
{
}



void GameObject::setMesh(Mesh * _mesh)
{
	mesh = _mesh;
}

void GameObject::setTexture(Texture * _texture)
{
	textures.clear();
	textures.push_back(TextureBind(_texture, 0));
}

void GameObject::setTextureBinds(std::vector<TextureBind> _textures)
{
	textures.clear();
	for (TextureBind texture : _textures)
		textures.push_back(texture);
}

void GameObject::setTextures(std::vector<Texture*> &_textures)
{
	textures.clear();
	int i = 0;
	for (Texture* texture : _textures)
		textures.push_back(TextureBind(texture, i++));
}

void GameObject::draw()
{
	for (TextureBind tex : textures)
	{
		tex.texture->bind(tex.bindSlot);
	}
	mesh->draw();
	for (TextureBind tex : textures)
	{
		tex.texture->unbind(tex.bindSlot);
	}
}
