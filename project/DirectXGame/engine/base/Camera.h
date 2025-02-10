#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Calculation.h"

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

class Camera
{
public:
	Camera();

	void Update();

	//Getter
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	Vector3& GetScale() { return transform_.scale; }
	Vector3& GetRotate() { return transform_.rotate; }
	Vector3& GetTranslate() { return transform_.translate; }

	//Setter
	void SetScale(const Vector3& scale) { transform_.scale = scale; }
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	void SetFovY(float fovY) { fovY_ = fovY; }
	void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(float nearClip) { nearClip_ = nearClip; }
	void SetFarClip(float farClip) { farClip_ = farClip; }

private:
	Transform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	float fovY_;
	float aspectRatio_;
	float nearClip_;
	float farClip_;
	Matrix4x4 viewProjectionMatrix_;
	
};

