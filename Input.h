#pragma once
#include <dinput.h>
#include <Windows.h>
#include<wrl.h>
//#define DIRECTINPUT_VERSION 0x0800;
#include<dinput.h>

class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize(HINSTANCE hInstance, HWND hwnd);

	void Update();

private:
	//キーボードデバイス生成
	ComPtr<IDirectInputDevice8>keyboard;
};

