#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Framebuffer.h"
#include "ResourceManager.h"

class PauseMenuMesh;

class TimeSplitterMenuScene : public Scene
{
public:
	TimeSplitterMenuScene(Game* game);
	~TimeSplitterMenuScene();

	void initializeScene();
	void update(float dt);
	void draw();
	void reshape(int width, int height);

private:
	Mesh meshCylinder;
	Mesh meshCylinderCut;

	std::vector<GameObject> goSpinningTextures;
	GameObject goTsBackground;
	GameObject goTsDates;
	GameObject goTsBlur;
	GameObject goTsSwirlTop;
	GameObject goTsSwirlBottom;

	GameObject goTsTextBottom;
	GameObject goTsTextBottomBlur;
	GameObject goTsTimeLines;

	GameObject goPauseGrid;
	GameObject goPauseShine;
	
	PauseMenuMesh* pauseEffect;
	Transform tsParent;
	Transform tsSpinParent;

	FrameBuffer* fBufferPause;

	FontFace* fontCommodore;
	TextRenderer textTest;

	ShaderProgram* shaderUnlit;
	ShaderProgram* shaderUnlitAlphaColor; 
	ShaderProgram* shaderPauseEffect;
};