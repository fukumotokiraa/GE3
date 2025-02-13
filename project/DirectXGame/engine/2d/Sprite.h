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

	const Vector2& GetAnchorPoint()const { return anchorPoint_; }
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	bool GetIsFlipX()const { return isFlipX_; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }

	bool GetIsFlipY()const { return isFlipY_; }
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; }

	const Vector2& GetTextureLeftTop()const { return textureLeftTop; }
	void SetTextureLeftTop(const Vector2& textureLeftTop) { this->textureLeftTop = textureLeftTop; }

	const Vector2& GetTextureSize()const { return textureSize; }
	void SetTextureSize(const Vector2& textureSize) { this->textureSize = textureSize; }

private:
	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

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

	Vector2 anchorPoint_ = { 0.0f,0.0f };

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;

	bool isChecked = true;

	//テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureSize = { 100.0f,100.0f };

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
	uint32_t textureIndex_ = 0;
	std::string textureFilePath_;

};

