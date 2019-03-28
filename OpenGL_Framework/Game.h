#pragma once
#define NOMINMAX
#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Timer.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Framebuffer.h"

#include "GameObject.h"
#include "PauseEffect.h"
#include "Scene.h"

#define FRAMES_PER_SECOND		60

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

class PauseMenuMesh;

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void keyboardSpecialDown(int key, int mouseX, int mouseY);
	void keyboardSpecialUp(int key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);
	void reshapeGame(int width, int height);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;
	bool guiEnabled = true;

private:
	std::vector<Scene*> _Scenes;
	Scene* _ActiveScene;


	// Scene Objects.
	Camera camera;
	

	
	// OpenGL Handles
	ShaderProgram* shaderPassthrough;
	ShaderProgram* shaderPassthroughColor;
	ShaderProgram* shaderPauseEffect;
	ShaderProgram* shader;
	ShaderProgram* shaderUnlit;
	ShaderProgram* shaderUnlitAlphaColor;
	ShaderProgram* shaderTexture;
	ShaderProgram* shaderWater;
	ShaderProgram* shaderTerrain;
	ShaderProgram* shaderACES;

	FrameBuffer* fBuffer;
};
