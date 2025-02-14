#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

class DirectXCommon;
class SrvManager
{
public:
	SrvManager();
	~SrvManager();
	void Initialize(DirectXCommon* dxCommon);

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	void CreateSRVforTexture2D(uint32_t srvIndex,ID3D12Resource* pResource,DXGI_FORMAT Format,UINT MipLevels);
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	void PreDraw();

	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	bool maxTextureCheck();

	//getter
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return descriptorHeap; }

	static const uint32_t kMaxSRVCount;
private:
	DirectXCommon* dxCommon_ = nullptr;

	uint32_t desriptorSize;
	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> descriptorHeap;

	uint32_t useIndex = 0;



};

