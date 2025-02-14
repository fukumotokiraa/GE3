#include "ImGuiManager.h"

#include "DirectXCommon.h"
#include "SrvManager.h"
#include "externals/imgui/imgui.h"
#include "WinApp.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

void ImGuiManager::Finalize()
{
#ifdef USE_IMGUI
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

void ImGuiManager::Initialize([[maybe_unused]] WinApp* winApp, [[maybe_unused]] DirectXCommon* dxCommon, [[maybe_unused]] SrvManager* srvManager)
{
#ifdef USE_IMGUI
	winApp_ = winApp;
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	srvIndex = srvManager_->Allocate();
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	//ImGuiのWin32の初期化
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	//ImGuiのDX12の初期化
	ImGui_ImplDX12_Init( dxCommon_->GetDevice().Get(), static_cast<int>(dxCommon_->GetSwapChainResourcesNum()), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvManager_->GetDescriptorHeap().Get(), srvManager_->GetCPUDescriptorHandle(srvIndex), srvManager_->GetGPUDescriptorHandle(srvIndex));
#endif
}

void ImGuiManager::Begin()
{
#ifdef USE_IMGUI
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
}

void ImGuiManager::End()
{
#ifdef USE_IMGUI
	ImGui::Render();
#endif
}

void ImGuiManager::Draw()
{
#ifdef USE_IMGUI
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandlist().Get();
	//デスクリプターヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvManager_->GetDescriptorHeap().Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif

}
