#include "ImGuiManager.h"
#include "externals/imgui/imgui.h"
#include "WinApp.h"
#include "externals/imgui/imgui_impl_win32.h"

void ImGuiManager::Initialize(WinApp* winApp)
{
	winApp_ = winApp;
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	//ImGuiのWin32の初期化
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
}
