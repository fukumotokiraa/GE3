#pragma once

#include <math.h>
#include <cassert>
#include <cmath>
//#include<imgui.h>
#define _USE_MATH_DEFINES
#include "math.h"
#include "Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4x4.h"

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v);

Vector3 Divide(const Vector3& v, float divisor);

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator*(float s, const Vector3& v);

Vector3 operator*(const Vector3& v, float s);

Vector3 operator/(const Vector3& v, float s);

Vector3 operator+=(Vector3& v1, const Vector3& v2);

Vector3 operator-=(Vector3& v1, const Vector3& v2);

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);

//Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

float Radian(float degree);

float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);

float Dot(const Vector3& a, const Vector3& b);

Vector3 Reflect(const Vector3& input, const Vector3& normal);

Vector3 Project(const Vector3& a, const Vector3& b);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Inverse(const Matrix4x4& m); // 逆行列

//Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Vector4 Transform4(const Vector4& vec, const Matrix4x4& mat);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Multiply(const Matrix4x4& m, const Vector4& v);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& radian, const Vector3& translate);

Matrix4x4 MakeRotateMatrix(Vector3 radian);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

class MyVector3 {
public:
	float x;
	float y;
	float z;

	MyVector3& operator*=(float& s);
	MyVector3& operator-=(const Vector3& v);
	MyVector3& operator+=(const Vector3& v);
	MyVector3& operator/=(float s);
};