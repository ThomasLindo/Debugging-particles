#include "TimeSplitterMenuScene.h"
#include "PauseEffect.h"
#include "Game.h"
#include "ResourceManager.h"
#include "PauseEffect.h"
#include "IO.h"
#include "Input.h"
#include "UI.h"

TimeSplitterMenuScene::TimeSplitterMenuScene(Game * game) : Scene(game)
{
	initializeScene();
}

TimeSplitterMenuScene::~TimeSplitterMenuScene()
{
	if (mainCamera)
		delete mainCamera;

	if (pauseEffect != nullptr)
	{
		delete pauseEffect;
		pauseEffect = nullptr;
	}
}

void TimeSplitterMenuScene::initializeScene()
{
	fBufferPause = new FrameBuffer(1);
	fBufferPause->InitColorTexture(0, Window::getWidth(), Window::getHeight());

	meshCylinder.initMeshCylinder(64U, 1U, true);
	meshCylinderCut.initMeshCylinder(4U, 1U, true, 360.0f / 16.0f);

	for (unsigned int i = 0; i < 16; ++i)
	{
		goSpinningTextures.push_back(GameObject());
		goSpinningTextures[i].setMesh(&meshCylinderCut);
		goSpinningTextures[i].setTexture(Texture::add("TimeSplitters/menu_background" + io::zeroPadNumber(i + 1, 2) + ".png", Texture::Clamp, Texture::Clamp));
		goSpinningTextures[i].setLocalRotY(i * -22.5f - 180 + 22.5f);
	}

	tsParent.addChild(&tsSpinParent);
	//tsSpinParent.setLocalPos(vec3(0.f, -0.5f, 0.f));
	tsSpinParent.setScale(vec3(1.0f, 1.0f, 1.0f));

	tsParent.addChild(&goTsBackground);
	goTsBackground.setMesh(&meshCylinder);
	goTsBackground.setTexture(Texture::add("TimeSplitters/menu_background00.png"));
	goTsBackground.setScale(vec3(5, 15, 5));

	for (GameObject &go : goSpinningTextures)
	{
		tsSpinParent.addChild(&go);
	}

	goTsDates.setMesh(&meshCylinder);
	goTsBlur.setMesh(&meshCylinder);
	goTsSwirlTop.setMesh(&meshCylinder);
	goTsSwirlBottom.setMesh(&meshCylinder);
	goTsTextBottom.setMesh(&meshCylinder);
	goTsTextBottomBlur.setMesh(&meshCylinder);
	goTsTimeLines.setMesh(&meshCylinder);
	goTsDates.setMesh(&meshCylinder);

	pauseEffect = new PauseMenuMesh();
	pauseEffect->initMeshPauseGrids(Texture::add("TimeSplitters/pause_grid.png"), Texture::add("TimeSplitters/pause_bloom.png"));

	goTsDates.setTexture(Texture::add("TimeSplitters/a/dates.png", Texture::Repeat, Texture::Clamp));
	goTsBlur.setTexture(Texture::add("TimeSplitters/a/blur.png", Texture::Repeat, Texture::Clamp));
	goTsSwirlTop.setTexture(Texture::add("TimeSplitters/a/swirl.png", Texture::Repeat, Texture::Clamp));
	goTsSwirlBottom.setTexture(Texture::add("TimeSplitters/a/swirl.png", Texture::Repeat, Texture::Clamp));
	goTsTextBottom.setTexture(Texture::add("TimeSplitters/a/textBottom.png", Texture::Repeat, Texture::Clamp));
	goTsTextBottomBlur.setTexture(Texture::add("TimeSplitters/a/textBottomBlur.png", Texture::Repeat, Texture::Clamp));
	goTsTimeLines.setTexture(Texture::add("TimeSplitters/a/timelineSections.png", Texture::Repeat, Texture::Clamp));

	goTsDates.setScale(vec3(1));
	goTsBlur.setScale(vec3(1));
	goTsSwirlTop.setScale(vec3(1, 0.5f, 1));
	goTsSwirlBottom.setScale(vec3(1, 0.5f, 1));
	goTsTextBottom.setScale(vec3(1));
	goTsTextBottomBlur.setScale(vec3(1, 1.2f, 1));
	goTsTimeLines.setScale(vec3(1));

	goTsDates.setLocalPos(vec3(0, 0, 0));
	goTsBlur.setLocalPos(vec3(0, 0, 0));
	goTsSwirlTop.setLocalPos(vec3(0, 1.3f, 0));
	goTsSwirlBottom.setLocalPos(vec3(0, -2, 0));
	goTsTextBottom.setLocalPos(vec3(0, -2, 0));
	goTsTextBottomBlur.setLocalPos(vec3(0, -2.2f, 0));
	goTsTimeLines.setLocalPos(vec3(0, 0, 0));

	tsParent.addChild(&goTsDates);
	tsParent.addChild(&goTsBlur);
	tsParent.addChild(&goTsSwirlTop);
	tsParent.addChild(&goTsSwirlBottom);

	tsParent.addChild(&goTsTextBottom);
	tsParent.addChild(&goTsTextBottomBlur);
	tsParent.addChild(&goTsTimeLines);

	tsParent.setScale(vec3(7, 1.5, 7));
	addChild(&tsParent);

	shaderPauseEffect = ResourceManager::addShaderProgramVertFrag("PassThroughColor.vert", "pauseEffect.frag");
	shaderUnlit = ResourceManager::addShaderProgramVertFrag("shader.vert", "shaderUnlit.frag");
	shaderUnlitAlphaColor = ResourceManager::addShaderProgramVertFrag("shader.vert", "shaderUnlitAlphaColor.frag");

	fontCommodore = FontManager::initNewFont("impact.ttf", 64);
	//fontCommodore = FontManager::initNewFont("CommodoreAngled.ttf", 48);
	textTest.fontface = fontCommodore;
	textTest.text = std::string("This is a test\nI don't even know if newline works\n(It didn't, but I added the functionality)");
	textTest.origin = vec2(800.0f, 400.0f);

	textTest.shake.amount = vec2(5.0f, 15.0f);
	textTest.shake.frequency = vec2(0.02f, 0.005f);
	textTest.shake.speed = vec2(8.0f);

	//textTest.shake.amount = vec2(2.0f);
	//textTest.shake.speed = vec2(23.21f, 30.0f);
	//textTest.shake.frequency = vec2(10.5f, 10.33f);

	textTest.init();

	mainCamera = new Camera();
	addChild(mainCamera);

	mainCamera->perspective(60.0f, Window::getAspect(), 0.05f, 1000.0f);
	mainCamera->setLocalPos(vec3(0.0f, -0.3f, 0.0f));
}

void TimeSplitterMenuScene::update(float dt)
{


	pauseEffect->updateMeshPauseGrids(dt);
	goTsBackground.setLocalRotY(sin(m_pTimeSinceLoad / 12 * f_pi) * 10);
	tsSpinParent.setLocalRotY(m_pTimeSinceLoad * 4);
	goTsDates.setLocalRotY(m_pTimeSinceLoad * -10);
	goTsBlur.setLocalRotY(m_pTimeSinceLoad * 7);
	goTsSwirlTop.setLocalRotY(m_pTimeSinceLoad * 12);
	goTsSwirlBottom.setLocalRotY(m_pTimeSinceLoad * 12);
	goTsTextBottom.setLocalRotY(m_pTimeSinceLoad * -5);
	goTsTextBottomBlur.setLocalRotY(m_pTimeSinceLoad * 3);
	goTsTimeLines.setLocalRotY(sin(m_pTimeSinceLoad / 12 * f_pi) * 20);


	// Give our Transforms a chance to compute the latest matrices
	//mainCamera->update(dt);

	//tsParent.update(dt);

	//if (io::Input::GetActionDown("pause"))
	//	pauseEffect->toggle();


	float rainbowSpeed = 2.5f;
	float rainbowFrequency = -0.01f;
	textTest.update(dt * !pauseEffect->getState());
	for (size_t i = 0; i < textTest.data.size(); ++i)
	{
		float offsetAmount = (textTest.data[i].x + textTest.data[i].y) * rainbowFrequency + m_pTimeSinceLoad * rainbowSpeed;
		textTest.data[i].color = vec4(
			sinf(offsetAmount),
			sinf(offsetAmount + f_pi / 3.0f * 2.0f),
			sinf(offsetAmount + f_pi / 3.0f * 4.0f),
			1.0f);
		textTest.data[i].color += vec4(1.0f);
		textTest.data[i].color *= 0.5f;
	}

	if (io::Input::GetKeyDown(io::Input::KeyCode::Space))
		pauseEffect->toggle();

	if (pauseEffect->getState())
	{
		m_pTimeSinceLoad -= dt;
	}

	Scene::update(dt);
}

void TimeSplitterMenuScene::draw()
{
	mainCamera->render();
	//vec3 lightPos = vec3(camera.getView() * vec4(meshLight.getLocalPos(), 1.0f));

	fBufferPause->Clear();
	fBufferPause->bind();
	glEnable(GL_DEPTH_TEST);

	shaderUnlit->bind();
	shaderUnlit->sendUniform("uModel", goTsBackground.getLocalToWorld());
	shaderUnlit->sendUniform("uColor", vec4(vec3(0.4906105f, 0.6732029f, 1.0f) * 0.5f, 1));
	shaderUnlit->sendUniform("uTexST", vec4(3, 1, 0, 0));
	goTsBackground.draw();

	shaderUnlitAlphaColor->bind();
	shaderUnlitAlphaColor->sendUniform("uColor", vec4(vec3(0.4906105f, 0.6732029f, 1.0f), 1.0f));
	shaderUnlitAlphaColor->sendUniform("uTexST", vec4(1, 1, 0, 0));

	for (GameObject go : goSpinningTextures)
	{
		shaderUnlitAlphaColor->sendUniform("uModel", go.getLocalToWorld());
		go.draw();
	}
	glDisable(GL_DEPTH_TEST);
	shaderUnlitAlphaColor->sendUniform("uTexST", vec4(12, 2, 0, 0));
	shaderUnlitAlphaColor->sendUniform("uModel", goTsDates.getLocalToWorld());
	goTsDates.draw();
	shaderUnlitAlphaColor->sendUniform("uTexST", vec4(16, 1, 0, 0));
	shaderUnlitAlphaColor->sendUniform("uModel", goTsBlur.getLocalToWorld());
	goTsBlur.draw();
	shaderUnlitAlphaColor->sendUniform("uTexST", vec4(6, 1, 0, 0));
	shaderUnlitAlphaColor->sendUniform("uModel", goTsSwirlTop.getLocalToWorld());
	goTsSwirlTop.draw();
	shaderUnlitAlphaColor->sendUniform("uModel", goTsSwirlBottom.getLocalToWorld());
	goTsSwirlBottom.draw();
	shaderUnlitAlphaColor->sendUniform("uTexST", vec4(10, 2, 0, -1));
	shaderUnlitAlphaColor->sendUniform("uModel", goTsTextBottom.getLocalToWorld());
	goTsTextBottom.draw();
	shaderUnlitAlphaColor->sendUniform("uTexST", vec4(10, 2, 0.25, -1));
	shaderUnlitAlphaColor->sendUniform("uModel", goTsTextBottomBlur.getLocalToWorld());
	goTsTextBottomBlur.draw();
	shaderUnlitAlphaColor->sendUniform("uTexST", vec4(7.5, 0.5, 0, 0));
	shaderUnlitAlphaColor->sendUniform("uModel", goTsTimeLines.getLocalToWorld());
	goTsTimeLines.draw();

	textTest.draw();

	glDisable(GL_BLEND);
	fBufferPause->unbind();
	shaderPauseEffect->bind();
	fBufferPause->bindAsTex(0);
	pauseEffect->draw();



#if ENABLE_GUI
	ImGui::Begin("Test 1");

	int currTextAlign = (int)(textTest.alignment);
	if (ImGui::Combo("Mode", &currTextAlign, "Left\0Center\0Right\0\0"))
	{
		textTest.alignment = (TextRenderer::Alignment)currTextAlign;
	}

	static char guiTextInput[1024 * 8] = "This is a test\nI don't even know if newline works\n(It didn't, but I added the functionality)";
	if (ImGui::InputTextMultiline("Text Test", guiTextInput, IM_ARRAYSIZE(guiTextInput)))
	{
		textTest.text = guiTextInput;
	}
	ImGui::End();
#endif
}

void TimeSplitterMenuScene::reshape(int width, int height)
{
	fBufferPause->reshape(width, height);
}
