#include "UI.h"
#include "Window.h"


bool UI::m_isInit = false;

void UI::InitImGUI()
{
	ImGui::CreateContext();
	TTK::internal::imguiInit();
	m_isInit = true;
}

void UI::Start(int windowWidth, int windowHeight)
{
	if (m_isInit)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(windowWidth);
		io.DisplaySize.y = static_cast<float>(windowHeight);
		ImGui::NewFrame();
	}
	else
	{
		printf("Imgui is not currently initialized");
	}
}

void UI::End()
{
	if (m_isInit)
	{
		ImGui::Render();
	}
	else
	{
		printf("Imgui is not currently intialized");
	}
}
