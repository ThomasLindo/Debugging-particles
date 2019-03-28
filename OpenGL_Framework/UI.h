#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl.h"

// Just have the gui always on
//#ifdef DEBUG_GUI
#define ENABLE_GUI true
//#endif

#define IM_ARRAYSIZE(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class UI
{
public:
	static void InitImGUI();
	static void Start(int windowWidth, int windowHeight);
	static void Start();
	static void End();
	static bool isInit;
private:
};