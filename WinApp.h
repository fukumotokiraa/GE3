#pragma once
#include<wrl.h>
#include<Windows.h>
#include"externals/imgui/imgui.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include<cstdint>

//WindowsAPI
class WinApp
{
public:
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	void Initialize();

	void Update();

	HWND GetHwnd()const { return hwnd; }

	HINSTANCE GetHinstance()const { return wc.hInstance; }
private:
	HWND hwnd = nullptr;
	WNDCLASS wc{};
};

