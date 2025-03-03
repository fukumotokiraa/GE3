#pragma once
#include "ModelCommon.h"
#include "TextureManager.h"
#include "Calculation.h"

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

class Model
{
public:
	void Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename);

	void Draw();

	//.mtlファイルの読み込み
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//.objファイルの読み込み
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	ModelData* GetModelData() {return &modelData_;}

	D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() { return &vertexBufferView; }

private:
	//ModelCommonのポインタ
	ModelCommon* modelCommon_;
	//Objファイルのデータ
	ModelData modelData_;
	//VertexResource
	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource = nullptr;
	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	//VertexResourceにデータを書き込むためのポインタ
	VertexData* vertexData = nullptr;
	//マテリアルリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource = nullptr;
	//マテリアルリソースにデータを書き込むためのポインタ
	Material* materialData = nullptr;

	uint32_t instanceCount = 10;

};

