#pragma once
#include<d3d12.h>
#include<dxcapi.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include"WinApp.h"
#include <format>
#include <array>
#include <chrono>
#include <thread>
#include"externals/DirectXTex/DirectXTex.h"

//DirectX基盤
class DirectXCommon
{
public://メンバ関数
	//デスクリプターヒープ生成関数
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	//DepthStencilTextureResource
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr < ID3D12Device>device, int32_t width, int32_t height);
	//シェーダーのコンパイル
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);
	//バッファリソースの生成
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
	//テクスチャリソースの生成
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateTextureResorce(Microsoft::WRL::ComPtr < ID3D12Device> device, const DirectX::TexMetadata& metadata);
	//テクスチャリソースの転送
	void UploadTextureData(Microsoft::WRL::ComPtr < ID3D12Resource>texture, const DirectX::ScratchImage& mipImages);

	//デストラクタ
	~DirectXCommon();
	//初期化
	void Initialize(WinApp* winApp);
	//デバイスの初期化
	void InitializeDevice();
	//コマンド関連の初期化
	void InitializeCommand();
	//スワップチェーンの生成
	void CreateSwapChain();
	//深度バッファの生成
	void CreateDepthBuffer(int32_t width, int32_t height);
	//各種デスクリプターヒープの生成
	void CreateDescriptorHeap();
	//DXCコンパイラの生成
	void CreateDXCCompiler();
	//レンダーターゲットビューの初期化
	void InitializeRenderTargetView();
	//深度ステンシルビューの初期化
	void InitializeDepthStencilView();
	//フェンスの生成
	void InitializeFence();
	//ビューポート矩形の初期化
	void InitializeViewPort();
	//シザリング矩形の初期化
	void InitializeScissorRect();
	//ImGuiの初期化
	void InitializeImGui();
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	//RTV専用のデスクリプタ取得関数
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);
	uint32_t GetDesriptorSizeRTV() { return desriptorSizeRTV; };
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> GetRTVDescriptorHeap() { return rtvDescriptorHeap; };
	//rtvDesc
	D3D12_RENDER_TARGET_VIEW_DESC GetRTVDesc() { return rtvDesc; }
	//DSV専用のデスクリプタ取得関数
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);
	uint32_t GetDesriptorSizeDSV() { return desriptorSizeDSV; };
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> GetDSVDescriptorHeap() { return dsvDescriptorHeap; };
	//getter
	Microsoft::WRL::ComPtr < ID3D12Device> GetDevice()const { return device.Get(); }
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> GetCommandlist()const { return commandList.Get(); }
	//デスクリプタハンドル取得関数
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//Fence
	HANDLE GetFenceEvent() { return fennceEvent; }
	//スワップチェーンリソースの数を取得
	size_t GetSwapChainResourcesNum()const { return swapChainResources.size(); }

private:
	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	Microsoft::WRL::ComPtr < ID3D12Debug1> debugController = nullptr;
	Microsoft::WRL::ComPtr < IDXGIAdapter4> useAdapter = nullptr;
	Microsoft::WRL::ComPtr < ID3D12InfoQueue> infoQueue = nullptr;
	//DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	//コマンドアロケーター
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator = nullptr;
	//コマンドリスト
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList = nullptr;
	//コマンドキュー
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue = nullptr;
	//IDXGISwapChain4
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain = nullptr;
	//リソース
	Microsoft::WRL::ComPtr < ID3D12Resource> resource = nullptr;
	//
	WinApp* winApp_ = nullptr;
	//デスクリプター
	uint32_t desriptorSizeRTV = 0;
	uint32_t desriptorSizeDSV = 0;
	//デスクリプターヒープ
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;
	//スワップチェーンリソース
	//Microsoft::WRL::ComPtr < ID3D12Resource> swapChainResources[2] = { nullptr };
	std::array<Microsoft::WRL::ComPtr < ID3D12Resource>, 2>swapChainResources;
	//スワップチェーンデスク
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//rtvHandle
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	//ビューポート
	D3D12_VIEWPORT viewport{};
	//シザー矩形
	D3D12_RECT scissorRect{};
	//dxcCompilerを初期化
	Microsoft::WRL::ComPtr < IDxcUtils> dxcUtils = nullptr;
	Microsoft::WRL::ComPtr < IDxcCompiler3> dxcCompiler = nullptr;
	//インクルードハンドラの生成
	Microsoft::WRL::ComPtr < IDxcIncludeHandler> includeHandler = nullptr;
	//rtv
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//リソースバリア
	D3D12_RESOURCE_BARRIER barrier{};
	//Fence
	Microsoft::WRL::ComPtr < ID3D12Fence> fence = nullptr;
	//FenceValue
	uint64_t fenceValue = 0;
	//FenceEvent
	HANDLE fennceEvent;
	//DepthStencilResource
	Microsoft::WRL::ComPtr < ID3D12Resource> depthStencilResource = nullptr;

};

