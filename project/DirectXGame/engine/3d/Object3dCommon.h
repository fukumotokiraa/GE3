#pragma once
#include"externals/DirectXTex/d3dx12.h"
#include "Camera.h"

class DirectXCommon;

//3Dオブジェクト共通部
class Object3dCommon
{
public:
	~Object3dCommon() {};
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	//共通描画設定
	void DrawCommonSetting();

	DirectXCommon* GetDxCommon()const { return dxCommon_; }

	Camera* GetDefaultCamera()const { return defaultCamera_; }

	void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

private:
	//ルートシグネチャの作成
	void CreateRootSignature();
	//グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();

private:
	DirectXCommon* dxCommon_;
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};


	Camera* defaultCamera_ = nullptr;
};

