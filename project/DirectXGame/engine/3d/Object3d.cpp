#include "Object3d.h"



#include "Calculation.h"
#include "Object3dCommon.h"


void Object3d::Initialize(Object3dCommon* object3dCommon,Model*model)
{
	object3dCommon_ = object3dCommon;
	this->model = model;
	camera_ = object3dCommon_->GetDefaultCamera();
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	CreateMaterialData();
	CreateTransformationMatrixData();
	CreateDirectionLightData();
}

void Object3d::Update()
{
	// 行列の更新
	//transform.rotate.y += 0.03f;
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if(camera_) {
		const Matrix4x4 viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}else {
		worldViewProjectionMatrix = worldMatrix;
	}
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;

}

void Object3d::Draw()
{
	object3dCommon_->GetDxCommon()->GetCommandlist()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	object3dCommon_->GetDxCommon()->GetCommandlist()->SetGraphicsRootConstantBufferView(3, shaderResource->GetGPUVirtualAddress());
	if (model) {
		model->Draw();
	}
}

void Object3d::CreateMaterialData()
{
	//MaterialResourceを作る
}

void Object3d::CreateTransformationMatrixData()
{
	//Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixResource = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	//単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();

}

void Object3d::CreateDirectionLightData()
{
	shaderResource = object3dCommon_->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	shaderResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;
}

void Object3d::SetModel(const std::string& filePath)
{
	model = ModelManager::GetInstance()->FindModel(filePath);
}
