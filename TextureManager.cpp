#include "TextureManager.h"
#include "StringUtility.h"
#include "DirectXCommon.h"

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

void TextureManager::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
	//srvの数と同数
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	//読み込み済みテクスチャを検索
	auto it = std::find_if(textureDatas.begin(), textureDatas.end(),[&](TextureData& textureData_){return textureData_.filePath_ == filePath; });
	if (it != textureDatas.end()) {
		return;
	}
	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kMaxSRVCount);

	//テクスチャファイルを呼んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//テクスチャデータを追加
	textureDatas.resize(textureDatas.size() + 1);
	//追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas.back();

	textureData.filePath_ = filePath;
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResorce(dxCommon_->GetDevice(), textureData.metadata);

	dxCommon_->UploadTextureData(textureData.resource, mipImages);

	//テクスチャデータの要素数番号をSRVのインデックスとする
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.srvHandleCPU = dxCommon_->GetSRVCPUDescriptorHandle(srvIndex);
	textureData.srvHandleGPU = dxCommon_->GetSRVGPUDescriptorHandle(srvIndex);

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
	auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [&](TextureData& textureData_) {return textureData_.filePath_ == filePath; });
	if (it != textureDatas.end()) {
		//読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex)
{
	// 範囲外指定違反チェック
	assert(textureIndex < textureDatas.size()); // テクスチャ番号が正常範囲内であるか

	// テクスチャデータの参照を取得
	TextureData& textureData = textureDatas[textureIndex];

	// GPUハンドルを返す
	return textureData.srvHandleGPU;
}
