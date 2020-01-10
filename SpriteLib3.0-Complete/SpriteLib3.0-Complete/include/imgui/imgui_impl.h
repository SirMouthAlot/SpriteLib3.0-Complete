#pragma once
#include "imgui/imgui.h"

namespace TTK
{
	namespace internal
	{
		void imguiInit();
		void imguiDraw(ImDrawData* draw_data);
	}
}