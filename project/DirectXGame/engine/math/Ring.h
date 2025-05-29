#pragma once
#include <cstdint>
#include <numbers>

#include "Vector2.h"

class Ring
{
public:
	void CreateRing();

	Vector2 GetPosition() const { return Position; }
	float GetRotation() const { return Rotation; }
	Vector2 GetScale() const { return Scale; }

private:
	const uint32_t kRingDivide = 32;
	const float kOuterRadius = 1.0f;
	const float kInnerRadius = 0.2f;
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide);
	Vector2 Position = { 0.0f, 0.0f };
	float Rotation = 0.0f;
	Vector2 Scale = { 1.0f, 1.0f };
};

