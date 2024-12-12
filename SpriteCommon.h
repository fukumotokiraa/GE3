#pragma once
#include"externals/DirectXTex/d3dx12.h"

class DirectXCommon;

class SpriteCommon
{
public:
	void Initialize(DirectXCommon* dxCommon);

	void DrawCommonSetting();

	DirectXCommon* GetDxCommon()const { return dxCommon_; }

private:
	//ルートシグネチャの作成
	void CreateRootSignature();
	//グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();

	DirectXCommon* dxCommon_;
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;

};

