#define _CRT_SECURE_NO_WARNINGS //Remove warnings from deprecated functions. Shut up, Microsoft.

#define NOMINMAX
#include <windows.h>
#include <iostream>
#include "Game.h"
#include "IO.h"

Game *theGame;
float errorCount = 0;

/* function DisplayCallbackFunction(void)
 * Description:
 *  - this is the openGL display routine
 *  - this draws the sprites appropriately
 */
void DisplayCallbackFunction(void)
{
	theGame->draw();
}

/* function void KeyboardCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is pressed
 */
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	theGame->keyboardDown(key, x, y);
}

void SpecialCallbackFunction(int key, int x, int y)
{
	theGame->keyboardSpecialDown(key, x, y);
}

void SpecialUpCallbackFunction(int key, int x, int y)
{
	theGame->keyboardSpecialUp(key, x, y);
}

/* function void KeyboardUpCallbackFunction(unsigned char, int, int)
 * Description:
 *   - this handles keyboard input when a button is lifted
 */
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	theGame->keyboardUp(key, x, y);
}

/* function TimerCallbackFunction(int value)
 * Description: 
 *  - this is called many times per second
 *  - this enables you to animate things
 *  - no drawing, just changing the state
 *  - changes the frame number and calls for a redisplay
 *  - FRAME_DELAY_SPRITE is the number of milliseconds to wait before calling the timer again
 */
void TimerCallbackFunction(int value)
{
	theGame->update();

	glutPostRedisplay();
	glutTimerFunc(FRAME_DELAY_SPRITE, TimerCallbackFunction, 0);
}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	theGame->mouseClicked(button,state, x, y);
	glutPostRedisplay();
}

/* function MouseMotionCallbackFunction()
 * Description:
 *   - this is called when the mouse is clicked and moves
 */
void MouseMotionCallbackFunction(int x, int y)
{
	theGame->mouseMoved(x, y);
	glutPostRedisplay();
}

void ReshapeFunction(int width, int height)
{
	theGame->reshapeGame(width, height);
}

void CALLBACK OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data)
{
	char buffer[9] = { '\0' };
	sprintf(buffer, "%.8x", id);

	std::string message("OpenGL(0x");
	message += buffer;
	message += "): ";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		message += "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		message += "Deprecated behavior";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		message += "Undefined behavior";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		message += "Portability issue";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		message += "Performance issue";
		break;
	case GL_DEBUG_TYPE_MARKER:
		message += "Stream annotation";
		break;
	case GL_DEBUG_TYPE_OTHER_ARB:
		message += "Other";
		break;
	}

	message += " \nSource: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		message += "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		message += "Window system";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		message += "Shader compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		message += "Third party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		message += "Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		message += "Other";
	}

	message += " \nSeverity: ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		message += "HIGH";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		message += "Medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		message += "Low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		message += "Notification";
	}

	message += " \n";
	message += msg;
	message += " \n";

	if (type == GL_DEBUG_TYPE_ERROR)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
	
	errorCount++;
//	std::cout << message << " Error #" << errorCount << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
}

void InitOpenGLDebugCallback()
{
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(OpenGLDebugCallback, NULL);

	// If you want to disable notifications from triggering the callback function, you can uncomment the code below
	
	//*
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	// */
}

int main(int argc, char **argv)
{
	/* initialize the window and OpenGL properly */
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextFlags(GLUT_CORE_PROFILE);
	glutInitWindowSize(Window::getWidth(), Window::getHeight());
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);

	glutCreateWindow("OpenGL Framework");

	glewExperimental = true;

	glewInit(); // gl* functions are captured from the drivers here.

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	/* set up our function callbacks */
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutSpecialFunc(SpecialCallbackFunction);
	glutSpecialUpFunc(SpecialUpCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);
	glutReshapeFunc(ReshapeFunction);
	

#ifdef _DEBUG
	InitOpenGLDebugCallback();
#endif

	/* init the game */
	theGame = new Game();
	theGame->initializeGame();

	/* start the game */
	glutMainLoop();
	return 0;
}