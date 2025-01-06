#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
#include "WinApp.h"
#include "TextureManager.h"

#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include <string>

class SpriteCommon;

class Sprite
{
public:
	void Initialize(SpriteCommon* spriteCommon, std::string textureFilePath);

	void Update();

	void Draw();

	void CreateVertexData();

	void CreateMaterialData();

	void CreateTransformationMatrixData();

	const Vector2& GetPosition()const { return position_; }
	void SetPosition(const Vector2& position) { position_ = position; }

	float GetRotation()const { return rotation_; }
	void SetRotation(float rotation) { rotation_ = rotation; }

	const Vector4& GetColor()const { return materialData->color; }
	void SetColor(const Vector4& color) { materialData->color = color; }

	const Vector2& GetSize()const { return size_; }
	void SetSize(const Vector2& size) { size_ = size; }

private:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	Transform transform;

	Vector2 position_ = { 0.0f,0.0f };
	float rotation_ = 0.0f;
	Vector2 size_ = { 640.0f,360.0f };

	bool isChecked = true;

	SpriteCommon* spriteCommon_ = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource = nullptr;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource = nullptr;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	//バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource = nullptr;
	//バッファリソース内のデータを指すポインタ
	Material* materialData = nullptr;

	//バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource = nullptr;
	//バッファリソース内のデータを指すポインタ
	TransformationMatrix* transformationMatrixData = nullptr;
	//テクスチャ番号
	uint32_t textureIndex = 0;

};

