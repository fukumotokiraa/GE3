#pragma once
#include<wrl.h>
#include <dinput.h>

using namespace Microsoft::WRL;

class Input
{
public:
	void Initialize();

	void Update();

private:
	HRESULT result;
	HINSTANCE hInstance;
	HWND hwnd;
};

