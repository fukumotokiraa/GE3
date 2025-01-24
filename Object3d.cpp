#include "Object3d.h"



#include "Calculation.h"
#include "Object3dCommon.h"


void Object3d::Initialize(Object3dCommon* object3dCommon,Model*model)
{
	object3dCommon_ = object3dCommon;
	this->model = model;

	CreateMaterialData();
	CreateTransformationMatrixData();
	CreateDirectionLightData();

	//Transform変数を作る
	transform={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
}

void Object3d::Update()
{
	// 行列の更新
	//transform.rotate.y += 0.03f;
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
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
