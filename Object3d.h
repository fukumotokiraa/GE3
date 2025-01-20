#pragma once
#include <d3d12.h>
#include <string>
#include <vector>
#include <wrl/client.h>
#include "DirectXCommon.h"
#include "TextureManager.h"

#include "Matrix4x4.h"
#include "Object3dCommon.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class Object3dCommon;
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct MaterialData {
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};
struct Material {
	Vector4 color;
	bool enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
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
	void Initialize(Object3dCommon* object3dCommon);

	void Update();

	void Draw();

	void CreateMaterialData();

	void CreateTransformationMatrixData();

	void CreateDirectionLightData();

	Transform& GetTransform() { return transform; }

	//.mtlファイルの読み込み
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//.objファイルの読み込み
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
private:
	Object3dCommon* object3dCommon_ = nullptr;

	//objファイルのデータ
	ModelData modelData;
	//バッファリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource = nullptr;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource = nullptr;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

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

