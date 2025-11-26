#pragma once
#include "BaseScene.h"



class Clear
{
public:
	void Initialize(SpriteCommon* spriteCommon);
	void Finalize();
	void Update();
	void Draw();

	bool GetClear() const { return isClear; }
	void SetClear(bool clear) { isClear = clear; }

	inline float easeOutCubic(float t) {
		return 1 - powf(1 - t, 3);
	}

private:
	Sprite* clearSprite = nullptr;

	enum class ClearSpriteMoveState {
		Idle,
		Entering,
		Staying,
		Exiting
	};

	ClearSpriteMoveState clearSpriteMoveState = ClearSpriteMoveState::Idle;
	float clearSpriteMoveTimer = 0.0f;
	const float clearEnterDuration = 1.0f;
	const float clearStayDuration = 0.5f;
	const float clearExitDuration = 1.0f;
	Vector3 clearStartPos = { 0.0f, -720.0f, -200.0f }; // 画面外上
	Vector3 clearCenterPos = { 0.0f, 0.0f, -200.0f };   // 画面中央
	Vector3 clearEndPos = { 0.0f, 720.0f, -200.0f };    // 画面外下

	bool isClear = false;

};

