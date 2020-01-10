#ifndef __UI_H__
#define __UI_H__

#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl.h"

class UI
{
public:
	static void InitImGUI();
	static void Start(int windowWidth, int windowHeight);
	static void End();
	static bool m_isInit;
};

#endif // !__UI_H__
