#pragma once
#include "imgui/imgui.h"

namespace sat
{
	namespace internal
	{
		void imguiInit();
		void imguiDraw(ImDrawData* draw_data);
	}
}