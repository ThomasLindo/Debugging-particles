#include "Game.h"
#include "IO.h"
#include "Input.h"
#include "UI.h"
#include "Font.h"

#include <vector>
#include <string>
#include <fstream>

#include "ResourceManager.h"

#include "PauseEffect.h"
#include "TimeSplitterMenuScene.h"
#include "CGAssignment3Scene.h"
#include "DefaultScene.h"

Game::Game()
{
	updateTimer = new Timer();
}

Game::~Game()
{
	delete updateTimer;
}

constexpr int frameTimeNumSamples = 600;
int frameTimeCurrSample = 0;
float frameTimeSamples[frameTimeNumSamples];

void Game::initializeGame()
{
	for (int i = 0; i < frameTimeNumSamples; ++i)
		frameTimeSamples[i] = 0.016f;

	ShaderProgram::initDefault();
	FrameBuffer::InitFullScreenQuad();
	FontManager::init();

	UI::InitImGUI();

	fBuffer = new FrameBuffer(1);
	fBuffer->InitColorTexture(0, Window::getWidth(), Window::getHeight());
	
	// These Render flags can be set once at the start (No reason to waste time calling these functions every frame).
	// Tells OpenGL to respect the depth of the scene. Triangles will not render when they are behind other geometry.
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	   
	// Basic clear color used by glClear().
	glClearColor(0, 0, 0, 0); // Black.
	
	_Scenes.push_back(new DefaultScene(this));
	_Scenes.push_back(new TimeSplitterMenuScene(this));
	_Scenes.push_back(new CGAssignment3Scene(this));
	_ActiveScene = _Scenes[0];
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	// Gather multiple samples of framerate
	frameTimeSamples[frameTimeCurrSample] = min(deltaTime, 0.1f);
	frameTimeCurrSample = (frameTimeCurrSample + 1) % frameTimeNumSamples;
	frameTimeSamples[frameTimeCurrSample] = 1.0f;

	_ActiveScene->update(deltaTime);

	io::Input::update();
}

void Game::draw()
{
#if ENABLE_GUI
	UI::Start();
#endif

	_ActiveScene->draw();

#if ENABLE_GUI
	// Framerate Visualizer
	ImGui::Begin("Framerate");
	{
		float averageFramerate = 0.0f;
		for (int i = 0; i < frameTimeNumSamples; ++i)
			averageFramerate += frameTimeSamples[i];

		averageFramerate = (averageFramerate - 1.0f) / (frameTimeNumSamples - 1);
		std::string milliseconds = std::to_string(averageFramerate * 1000.0f) + "ms";
		averageFramerate = 1.0f / averageFramerate;
		std::string framerate = "Framerate: " + std::to_string(averageFramerate) + "\n" + milliseconds;

		ImGui::PlotHistogram("", frameTimeSamples, frameTimeNumSamples, 0, framerate.c_str(), 0.005f, 0.2f, ImVec2(frameTimeNumSamples, 48));
	}
	ImGui::End();

	ImGui::Begin("Scene Selector");
	const char* scenes[] = { "Default Scene", "TimeSplitters", "Assignment 3" };
	static int activeScene = 0;
	if (ImGui::Combo("Scene Select", &activeScene, scenes, IM_ARRAYSIZE(scenes)))
	{
		_ActiveScene = _Scenes[activeScene];
	}
	ImGui::Text("Also Test GUI");
	ImGui::End();
	UI::End();
#endif

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
#if ENABLE_GUI
	if (guiEnabled)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (key < 'a' || key > 'z')
			io.KeysDown[(int)key] = true;
		io.AddInputCharacter((int)key); // this is what makes keyboard input work in imgui
		// This is what makes the backspace button work
		int keyModifier = glutGetModifiers();
		switch (keyModifier)
		{
		case GLUT_ACTIVE_SHIFT:
			io.KeyShift = true;
			break;

		case GLUT_ACTIVE_CTRL:
			io.KeyCtrl = true;
			break;

		case GLUT_ACTIVE_ALT:
			io.KeyAlt = true;
			break;
		}
	}
	
	if (!ImGui::IsAnyItemHovered())
#endif
	{
		io::Input::processInput(key, true);
		vec3 positionOffset = vec3(0.f);
		switch (key)
		{
		case ' ':
			break;
		case 27: // the escape key
		case 'q':
			positionOffset.y -= 0.1f;
			break;
		case 'e':
			positionOffset.y += 0.1f;
			break;
		case 'a':
			positionOffset.x -= 0.1f;
			break;
		case 'd':
			positionOffset.x += 0.1f;
			break;
		case 'w':
			positionOffset.z -= 0.1f;
			break;
		case 's':
			positionOffset.z += 0.1f;
			break;
		}
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
#if ENABLE_GUI
	if (guiEnabled)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		int keyModifier = glutGetModifiers();
		io.KeyShift = false;
		io.KeyCtrl = false;
		io.KeyAlt = false;
		switch (keyModifier)
		{
		case GLUT_ACTIVE_SHIFT:
			io.KeyShift = true;
			break;

		case GLUT_ACTIVE_CTRL:
			io.KeyCtrl = true;
			break;

		case GLUT_ACTIVE_ALT:
			io.KeyAlt = true;
			break;
		}
	}
#endif

	io::Input::processInput(key, false);
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
		exit(1);
		break;
	}
}

void Game::keyboardSpecialDown(int key, int mouseX, int mouseY)
{
	io::Input::processInput(key, true);
	switch (key)
	{
	case GLUT_KEY_F5:
		ResourceManager::reloadShaders();
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;
	case GLUT_KEY_END:
		exit(1);
		break;
	}
}

void Game::keyboardSpecialUp(int key, int mouseX, int mouseY)
{
	io::Input::processInput(key, false);
}

void Game::mouseClicked(int button, int state, int x, int y)
{
#if ENABLE_GUI
	if (guiEnabled)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
		if(button < 3)
		{
			// GLUT organizes by left, middle, right click
			// ImGui organizes by left, right, middle
			// have to swap the 2 here
			switch (button)
			{
			case GLUT_LEFT_BUTTON:
				ImGui::GetIO().MouseDown[0] = !state;
				break;
			case GLUT_MIDDLE_BUTTON:
				ImGui::GetIO().MouseDown[2] = !state;
				break;
			case GLUT_RIGHT_BUTTON:
				ImGui::GetIO().MouseDown[1] = !state;
				break;
			}
		}
		if (state == GLUT_DOWN)
		{
			if (button == 3)
				ImGui::GetIO().MouseWheel += 1.0f;
			else if (button == 4)
				ImGui::GetIO().MouseWheel -= 1.0f;

		}
	}
#endif

	if(state == GLUT_DOWN) 
	{
		
	}
	else
	{

	}
}

void Game::mouseMoved(int x, int y)
{

#if ENABLE_GUI
	if (guiEnabled)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}
#endif
}

 void Game::reshapeGame(int width, int height)
 {
	 Window::setResolution(width, height);
	 fBuffer->reshape(width, height);
	 _ActiveScene->reshape(width, height);
 }
