#pragma once
#include <d3d12.h>
#include <string>
#include <vector>
#include <wrl/client.h>
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"

#include "Matrix4x4.h"
#include "Model.h"
#include "Object3dCommon.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class Object3dCommon;

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
//3Dオブジェクト
class Object3d
{
public:
	//初期化
	void Initialize(Object3dCommon* object3dCommon,Model* model);

	void Update();

	void Draw();

	void CreateMaterialData();

	void CreateTransformationMatrixData();

	void CreateDirectionLightData();

	Transform& GetTransform() { return transform; }

	void SetModel(Model* model) { this->model = model; }

	void SetModel(const std::string& filePath);

	void SetScale(const Vector3& scale) { transform.scale = scale; }
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }

	const Vector3& GetScale()const { return transform.scale; }
	const Vector3& GetRotate()const { return transform.rotate; }
	const Vector3& GetTranslate()const { return transform.translate; }


private:
	Object3dCommon* object3dCommon_ = nullptr;

	Model* model = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource = nullptr;
	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* transformationMatrixData = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> shaderResource = nullptr;
	//バッファリソース内のデータを指すポインタ
	DirectionalLight* directionalLightData = nullptr;

	Transform transform;
	Transform cameraTransform;
};

