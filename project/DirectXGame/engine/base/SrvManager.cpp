#include "SrvManager.h"
#include "SrvManager.h"
#include "SrvManager.h"
#include "SrvManager.h"
#include "DirectXCommon.h"

const uint32_t SrvManager::kMaxSRVCount = 512;

SrvManager::SrvManager()
{
}

SrvManager::~SrvManager()
{
}

void SrvManager::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;

	//デスクリプターヒープ生成
	descriptorHeap = dxCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	//デスクリプタ1個分のサイズを取得して記録
	desriptorSize = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t SrvManager::Allocate()
{
	//上限に達していないかチェックしてassert
	assert(useIndex < kMaxSRVCount);
	//returnする番号を一旦記録しておく
	int index = useIndex;
	//次回のために番号を1進める
	useIndex++;
	//上で記録した番号をreturn
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index)
{
	//デスクリプタヒープの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//デスクリプタヒープの先頭からindex番目のデスクリプタの位置を計算
	handleCPU.ptr += (desriptorSize * index);
	//計算した位置をreturn
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index)
{
	//デスクリプタヒープの先頭を取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	//デスクリプタヒープの先頭からindex番目のデスクリプタの位置を計算
	handleGPU.ptr += (desriptorSize * index);
	//計算した位置をreturn
	return handleGPU;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	//srvDescの各項目を埋める
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));

}

void SrvManager::PreDraw()
{
	//SRV用のデスクリプターヒープを指定する
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> descriptorHeaps[] = { descriptorHeap };
	dxCommon_->GetCommandlist()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
}

void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex)
{
	//ルートパラメータの設定
	dxCommon_->GetCommandlist()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}

bool SrvManager::maxTextureCheck()
{
	//テクスチャの枚数が上限に達していなければtrue
	return useIndex < kMaxSRVCount;
}
