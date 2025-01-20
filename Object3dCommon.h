#pragma once
#include"externals/DirectXTex/d3dx12.h"

class DirectXCommon;

//3Dオブジェクト共通部
class Object3dCommon
{
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	//共通描画設定
	void DrawCommonSetting();

	DirectXCommon* GetDxCommon()const { return dxCommon_; }

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
};

