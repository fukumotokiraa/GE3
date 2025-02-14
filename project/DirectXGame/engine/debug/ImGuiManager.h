#pragma once
#include <cstdint>
#ifdef USE_IMGUI
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#endif

class WinApp;
class DirectXCommon;
class SrvManager;
class ImGuiManager
{
public:
	void Finalize();

	void Initialize(WinApp* winApp, DirectXCommon* dxCommon, SrvManager* srvManager);

	void Begin();

	void End();

	void Draw();

private:
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	uint32_t srvIndex = 0;
};

