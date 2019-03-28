#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Framebuffer.h"
#include "ResourceManager.h"

class CGAssignment3Scene : public Scene
{
public:
	CGAssignment3Scene(Game * game);
	void initializeScene();
	void update(float dt);
	void draw();
	void reshape(int width, int height);

	Mesh meshSphere;
	Mesh meshPlane;

	Texture* tex3D;
	Texture* texNoise;
	Texture* texWater;
	Texture* texWaterSpecular;

	GameObject goTerrain;
	GameObject goWater;

	ShaderProgram* shaderWater;
	ShaderProgram* shaderTerrain;
};