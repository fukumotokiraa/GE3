#pragma once

class WinApp;
class ImGuiManager
{
public:
	void Initialize(WinApp* winApp);

private:
	WinApp* winApp_ = nullptr;;
};

