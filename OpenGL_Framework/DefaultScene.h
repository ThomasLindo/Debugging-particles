#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Framebuffer.h"
#include "ResourceManager.h"
#include "ParticleEmitter.h"

class DefaultScene : public Scene
{
public:
	DefaultScene(Game* game);
	~DefaultScene();

	void initializeScene();
	void update(float dt);
	void draw();
	void reshape(int width, int height);

private:

	Transform cameraPivot;
	ParticleEmitter particles;
	GameObject goPlane;
		
	FontFace* fontCommodore;
	TextRenderer textTest;

	ShaderProgram* shaderParticle;
	ShaderProgram* shaderUnlit;
	ShaderProgram* shaderUnlitAlphaColor; 
};