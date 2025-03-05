#include "ParticleManager.h"

#include "Logger.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "Object3dCommon.h"

ParticleManager* ParticleManager::instance = nullptr;
void ParticleManager::CreateRootSignature()
{
	HRESULT hr;

	//RootSignatureを作成
	descriptorRange[0].BaseShaderRegister = 0;//0から始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号０とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters;//√パラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ

	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;//ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;//レジスタ番号０を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr <ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	//InputLayout
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendStateの設定
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//RasiterzerStateの設定
	//カリングしない(裏面も表示させる)
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//shaderをコンパイルする
	vertexShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Particle.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Particle.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	//DepthStencilStateの設定
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みをします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;//D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void ParticleManager::CreateGraphicsPipeline()
{
	HRESULT hr;
	//RootSignatureを作成
	CreateRootSignature();

	//PSOを生成する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//PSOを作成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager,Object3dCommon*object3dCommon)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;
	object3dCommon_ = object3dCommon;

    std::random_device seedGenerator;
    randomEngine_.seed(seedGenerator());
	cameraTransform_={ {1.0f,1.0f,1.0f},{std::numbers::pi_v<float> / 3,std::numbers::pi_v<float>,0.0f},{0.0f,23.0f,10.0f} };
	//パイプライン生成
	CreateGraphicsPipeline();


	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();

	camera_ = object3dCommon_->GetDefaultCamera();

	accelerationField.acceleration = { 10.0f,0.0f,0.0f };
	accelerationField.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField.area.max = { 1.0f,1.0f,1.0f };
}

void ParticleManager::Update()
{

	//ビルボード行列の計算
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);

	//ビュー行列とプロジェクション行列をカメラから取得
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);

	//全パーティクルグループ内の全パーティクルについて二重for文で処理
	for (auto& [name,particleGroup] : particleGroups_) {
		uint32_t count = 0;
		for (auto it = particleGroup.particles.begin(); it != particleGroup.particles.end();) {
			//寿命に達していたらグループから外す
			if ((*it).currentTime >= (*it).lifeTime) {
				it = particleGroup.particles.erase(it);
				continue;
			}

			//加速場
			if (applyField && isCollision(accelerationField.area, (*it).transform.translate)) {
				(*it).velocity += accelerationField.acceleration * kDeltaTime_;
			}
			//移動処理
			(*it).transform.translate += (*it).velocity * kDeltaTime_;
			//経過時間を加算
			(*it).currentTime += kDeltaTime_;
			//徐々に消えていく
			float alpha = 1.0f - (*it).currentTime / (*it).lifeTime;
			
			//ワールド行列を計算
			Matrix4x4 worldMatrix = MakeAffineMatrix((*it).transform.scale, (*it).transform.rotate, (*it).transform.translate);
			//ビルボード行列を合成
			if (useBillBoard) {
				Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
				Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
				billboardMatrix.m[3][0] = 0.0f;
				billboardMatrix.m[3][1] = 0.0f;
				billboardMatrix.m[3][2] = 0.0f;

				worldMatrix = Multiply(worldMatrix, billboardMatrix);
			}
			//ワールドビュープロジェクション行列を合成
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
			//インスタンシング用データ1個分の書き込み
			if (count < particleGroup.instanceCount) {
				particleGroup.instanceData[count].WVP = worldViewProjectionMatrix;
				particleGroup.instanceData[count].world = worldMatrix;
				particleGroup.instanceData[count].color = (*it).color;
				particleGroup.instanceData[count].color.w = alpha;
				count++;
			}
			it++;
		}
		//instanceCountを設定
		particleGroup.instanceCount = count;

		if (particleGroup.instanceCount == 0) {
			memset(particleGroup.instanceData, 0, sizeof(ParticleForGPU) * 1024);
		}
	}

}

void ParticleManager::Draw()
{
	//パーティクルグループが設定されていなければ描画しない
	if (particleGroups_.empty()) {
		return;
	}
	//ルートシグネチャを設定
	dxCommon_->GetCommandlist()->SetGraphicsRootSignature(rootSignature.Get());
	//パイプラインステートを設定
	dxCommon_->GetCommandlist()->SetPipelineState(graphicsPipelineState.Get());
	//プリミティブトポロジーを設定
	dxCommon_->GetCommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//全てのパーティクルグループについて処理する
	for (const auto& [name,particleGroup] : particleGroups_) {
		if(particleGroup.instanceCount==0) {
			continue;
		}

		//VertexBufferViewを設定
		vertexBufferView = model_->GetVertexBufferView();

		dxCommon_->GetCommandlist()->IASetVertexBuffers(0, 1, vertexBufferView);
		dxCommon_->GetCommandlist()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
		//テクスチャのSRVのDescriptorTableを設定
		dxCommon_->GetCommandlist()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(particleGroup.materialData.textureFilePath));
		//インスタンシングデータのSRVのDescriptorTableを設定
		dxCommon_->GetCommandlist()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(particleGroup.instanceSrvIndex));
		//描画(DrawCall)
		dxCommon_->GetCommandlist()->DrawInstanced(UINT(model_->GetModelData()->vertices.size()), particleGroup.instanceCount, 0, 0);
	}
}

void ParticleManager::CreateParticleGroup(const std::string& name, const std::string& textureFilePath, const std::string& modelFilePath)
{
	ModelManager::GetInstance()->LoadModel(modelFilePath);
	model_ = ModelManager::GetInstance()->FindModel(modelFilePath);

	if(particleGroups_.contains(name)) {
		return;
	}

	//パーティクルグループを作成し、コンテナに登録
	ParticleGroup particleGroup = {};
	particleGroups_.insert(std::make_pair(name, std::move(particleGroup)));
	//テクスチャファイルパスを登録
	particleGroups_.at(name).materialData.textureFilePath = textureFilePath;
	//テクスチャを読み込み、SRVを作成
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	//SRVインデックスを登録
	particleGroups_.at(name).materialData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
	//最大インスタンス数
	uint32_t MaxInstanceCount = 1024;
	//インスタンス鵜を初期化
	particleGroups_.at(name).instanceCount = 0;
	//インスタンスリソースを作成
	particleGroups_.at(name).instanceResource = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * MaxInstanceCount);
	//インスタンスリソースをマップ
	particleGroups_.at(name).instanceResource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroups_.at(name).instanceData));
	//インスタンスのデータを初期化
	particleForGPU.WVP = MakeIdentity4x4();
	particleForGPU.world = MakeIdentity4x4();
	particleForGPU.color = { 1.0f,1.0f,1.0f,1.0f };
	//インスタンスのデータを登録
	for (uint32_t i = 0; i < MaxInstanceCount; i++) {
		particleGroups_.at(name).instanceData[i] = particleForGPU;
	}
	//インスタンス用のSRVインデックス
	particleGroups_.at(name).instanceSrvIndex = srvManager_->Allocate();
	//インスタンス用のSRVを作成
	srvManager_->CreateSRVforStructuredBuffer(particleGroups_.at(name).instanceSrvIndex, particleGroups_.at(name).instanceResource.Get(), MaxInstanceCount, sizeof(ParticleForGPU));

	
}

void ParticleManager::Emit(const std::string& name, const Vector3& position, uint32_t count)
{
	assert(particleGroups_.contains(name));
	ParticleGroup& group = particleGroups_.at(name);

	// パーティクルグループのパーティクルリストに新しいパーティクルを追加
	for (uint32_t i = 0; i < count; i++) {
		group.particles.push_back(MakeNewParticle(randomEngine_, position));
	}

	// インスタンスカウントを更新
	group.instanceCount += count;
}

Particle ParticleManager::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate)
{
	std::uniform_real_distribution<float>distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float>distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float>distTime(1.0f, 3.0f);
	Particle particle;
	Vector3 randomTranslate{ distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = translate + randomTranslate;
	particle.velocity = { distribution(randomEngine),distribution(randomEngine), distribution(randomEngine) };
	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}
