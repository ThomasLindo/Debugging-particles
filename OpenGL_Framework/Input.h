#pragma once
#include <unordered_map>

// This was a mess and I regret everything

namespace io
{
	class Input
	{
	public:
		enum class KeyCode
		{
			None = 0,

			Backspace,
			Tab,
			Enter,
			Pause,
			Escape,
			Space,

			Num0,
			Num1,
			Num2,
			Num3,
			Num4,
			Num5,
			Num6,
			Num7,
			Num8,
			Num9,

			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,

			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,

			LBracket,
			RBracket,

			Up,
			Down,
			Right,
			Left,

			LShift,
			RShift,
			Shift,
			LCtrl,
			RCtrl,
			Ctrl,
			LAlt,
			RAlt,
			Alt,

			Insert,
			Home,
			End,
			PageUp,
			PageDown,
			Delete,

			Mouse0,	// Left/Primary Mouse Button
			Mouse1, // Right/Secondary Mouse Button
			Mouse2, // Middle Mouse Button
			Mouse3, // 4th Mouse Button
			Mouse4, // 5th Mouse Button
			Mouse5,
			Mouse6,


			LastElement
		};


		struct KeyState
		{
			bool release = false;
			bool pressed = false;
			bool held = false;
		};

		struct Action
		{
			std::string name;
			std::vector<KeyCode> keys;
			KeyState state;
			void addActionTrigger(KeyCode key);
		};

		static bool GetKeyDown(KeyCode key);
		static bool GetKey(KeyCode key);
		static bool GetKeyUp(KeyCode key);

		static bool GetActionDown(const std::string &key);
		static bool GetAction(const std::string &key);
		static bool GetActionUp(const std::string &key);

		static void update();
		static void processInput(int key, bool down);
		static void processInput(unsigned char key, bool down);
		static void addAction(std::string action);
		static std::unordered_map<KeyCode, KeyState> input;
		static std::unordered_map<std::string, Action> actions;
	private:
		static KeyCode GLUTtoKey(unsigned char key);
	};
}