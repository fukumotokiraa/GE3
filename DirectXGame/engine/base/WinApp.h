#pragma once
#include<wrl.h>
#include<Windows.h>
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

	void Finalize();

	bool ProcessMessage();

	HWND GetHwnd()const { return hwnd; }

	HINSTANCE GetHinstance()const { return wc.hInstance; }
private:
	HWND hwnd = nullptr;
	WNDCLASS wc{};
};

