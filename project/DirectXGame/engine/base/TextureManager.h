#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#include "externals/DirectXTex/DirectXTex.h"
#include "DirectXCommon.h"

#include <string>
#include <unordered_map>

class DirectXCommon;
class SrvManager;

class TextureManager
{
private:
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

public:
	static TextureManager* GetInstance();

	void Finalize();

	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);
	//テクスチャファイルの読み込み
	void LoadTexture(const std::string& filePath);
	//srvインデックスの開始番号
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	//テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);
	//メタデータを取得
	const DirectX::TexMetadata& GetMetadata(const std::string& filePath);
	//srvインデックスの取得
	uint32_t GetSRVIndex(const std::string& filePath);

private:
	DirectXCommon* dxCommon_;
	SrvManager* srvManager_;

	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	std::unordered_map<std::string, TextureData>textureDatas;

	//srvインデックスの開始番号
	static uint32_t kSRVIndexTop;

};

