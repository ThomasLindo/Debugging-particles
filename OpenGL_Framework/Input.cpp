#include "Input.h"
#include "IO.h"
#include <locale>
#include <GL/freeglut.h>

#define checkMap(i, o)		i.find(o) != i.end()

namespace io
{
	std::unordered_map<unsigned char, Input::KeyCode> generateMapGLUT()
	{
		std::unordered_map<unsigned char, Input::KeyCode> temp;
		for (unsigned char i = 0; i < 26; ++i)
		{
			temp['a' + i]		= (Input::KeyCode)((unsigned char)Input::KeyCode::A + i);
			temp['A' + i]		= (Input::KeyCode)((unsigned char)Input::KeyCode::A + i);
			temp['A' + i - 64]	= (Input::KeyCode)((unsigned char)Input::KeyCode::A + i);
		}
		for (unsigned char i = 0; i < 10; ++i)
		{
			temp['0' + i] = (Input::KeyCode)((unsigned char)Input::KeyCode::Num0 + i);
		}
		temp[' '] = Input::KeyCode::Space;
		return temp;
	}
	std::unordered_map<unsigned char, Input::KeyCode> glut2Key = generateMapGLUT();


	std::unordered_map<Input::KeyCode, Input::KeyState> generateInputMap()
	{
		std::unordered_map<Input::KeyCode, Input::KeyState> temp;
		for (Input::KeyCode i = Input::KeyCode::None; i < Input::KeyCode::LastElement; i = (Input::KeyCode)((int)i+1))
		{
			temp[i] = Input::KeyState();
		}
		return temp;
	}
	std::unordered_map<Input::KeyCode, Input::KeyState> Input::input = generateInputMap();
	std::unordered_map<std::string, Input::Action> Input::actions;


	bool Input::GetKeyDown(KeyCode key)
	{
		if (checkMap(input, key))
		{
			KeyState* state = &input.at(key);
			return state->pressed;
		}
		SAT_DEBUG_LOG_ERROR("KEY NOT CONFIGURED: %d", key);
		return false;
	}

	bool Input::GetKey(KeyCode key)
	{
		if (checkMap(input, key))
		{
			KeyState* state = &input.at(key);
			return state->held;
		}
		SAT_DEBUG_LOG_ERROR("KEY NOT CONFIGURED: %d", key);
		return false;
	}

	bool Input::GetKeyUp(KeyCode key)
	{
		if (checkMap(input, key))
		{
			KeyState* state = &input.at(key);
			return state->release;
		}
		SAT_DEBUG_LOG_ERROR("KEY NOT CONFIGURED: %d", key);
		return false;
	}

	bool Input::GetActionDown(const std::string & key)
	{
		std::string keyLow = toLower(key);
		if (checkMap(actions, keyLow))
		{
			KeyState* state = &actions.at(keyLow).state;
			if (state->held && !state->pressed)
			{
				state->pressed = true;
				return true;
			}
		}

		SAT_DEBUG_LOG_WARNING("Key: \'%s\' does not exist!", key.c_str());
		return false;
	}

	bool Input::GetAction(const std::string & key)
	{
		return false;
	}

	bool Input::GetActionUp(const std::string & key)
	{
		return false;
	}

	void Input::update()
	{
		std::unordered_map<KeyCode, KeyState>::iterator it = input.begin();
		while (it != input.end())
		{
			KeyState* state = &it->second;
			if (state->pressed)
			{
				state->pressed = false;
			}
			it++;
		}
	}

	void Input::processInput(int key, bool down)
	{
		if (down)
		{

		}
		else
		{

		}
	}

	void Input::processInput(unsigned char key, bool down)
	{
		KeyCode keycode = GLUTtoKey(key);
		if (checkMap(input, keycode))
		{
			KeyState* state = &input.at(keycode);
			if (down)
			{
				if (!state->held)
				{
					state->held = true;
					state->pressed = true;
					state->release = false;
				}
			}
			else
			{
				state->held = false;
				state->pressed = false;
				state->release = true;
			}
		}
		else
		{
			SAT_DEBUG_LOG_ERROR("KEY NOT CONFIGURED: %d", key);
		}
	}

	void Input::addAction(std::string action)
	{
		actions[action].name = action;
	}

	Input::KeyCode Input::GLUTtoKey(unsigned char key)
	{
		if(checkMap(glut2Key, key))
		{
			return glut2Key.at(key);
		}
		else
		{
			return glut2Key.at(' ');
		}
	}

	void Input::Action::addActionTrigger(KeyCode key)
	{
		keys.push_back(key);
	}
}