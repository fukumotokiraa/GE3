#include "TextureManager.h"

#include <SrvManager.h>

#include "StringUtility.h"
#include "DirectXCommon.h"

#include <filesystem>
#include <algorithm>

TextureManager* TextureManager::instance = nullptr;

//ImGuiで0番から使用するため1番から使用
uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if(instance==nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;
	//srvの数と同数
	textureDatas.reserve(SrvManager::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	if(textureDatas.contains(filePath)) {
		return;
	}
	assert(srvManager_->maxTextureCheck());

	// テクスチャファイルを呼んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);

	// 拡張子判定（小文字化して比較）
	std::filesystem::path fsPath(filePath);
	std::string ext = fsPath.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return static_cast<char>(std::tolower(c)); });

	HRESULT hr = S_OK;
	if (ext == ".dds") {
		// DDS ファイルは DirectXTex の DDS ローダを使う
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
		assert(SUCCEEDED(hr));
	} else {
		// WIC 対応フォーマット（png, jpg, etc.）
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
		assert(SUCCEEDED(hr));
	}

	// ミニマップの作成（既に mipLevels を持っている場合は生成をスキップ）
	DirectX::ScratchImage mipImages{};
	const DirectX::TexMetadata& srcMeta = image.GetMetadata();
	if (srcMeta.mipLevels > 1) {
		// DDS に既にミップマップが含まれている（または元データが mip を持つ）場合はそのまま使う
		mipImages = std::move(image);
	} else {
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
		assert(SUCCEEDED(hr));
	}

	// テクスチャデータを追加
	TextureData& textureData = textureDatas[filePath];

	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResorce(dxCommon_->GetDevice(), textureData.metadata);

	dxCommon_->UploadTextureData(textureData.resource, mipImages);

	// テクスチャデータの要素数番号をSRVのインデックスとする
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	//srvの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	//srv生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	//読み込み済みテクスチャデータを検索
	if (textureDatas.contains(filePath)) {
		//読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), textureDatas.find(filePath)));
		return textureIndex;
	}
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath)
{
	// 範囲外指定違反チェック
	assert(textureDatas.contains(filePath)); // テクスチャ番号が正常範囲内であるか

	// テクスチャデータの参照を取得
	TextureData& textureData = textureDatas[filePath];

	// GPUハンドルを返す
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetadata(const std::string& filePath)
{
	//範囲外指定違反チェック
	assert(textureDatas.contains(filePath));//テクスチャ番号が正常範囲内であるか

	TextureData& textureData = textureDatas[filePath];
	return textureData.metadata;
}

uint32_t TextureManager::GetSRVIndex(const std::string& filePath)
{
	//範囲外指定違反チェック
	assert(textureDatas.contains(filePath));//テクスチャ番号が正常範囲内であるか
	TextureData& textureData = textureDatas[filePath];
	return textureData.srvIndex;
}
