#pragma once
#include "Framework.h"
#include "Sprite.h"
#include "Model.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "Stage.h"
#include "Scene/GameScene.h"
#include "Scene/TitleScene.h"
#include "Scene/SceneManager.h"

inline float easeOutCubic(float t) {
return 1 - powf(1 - t, 3);
}

class MyGame : public Framework
{
public:
	MyGame();
	~MyGame();

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	SceneManager* sceneManager_ = nullptr;
	Sprite* sprite = nullptr;
	std::vector<Sprite*>sprites;
	Model* model = nullptr;
	Object3d* object3d = nullptr;
	Model* model2 = nullptr;
	Object3d* object3d2 = nullptr;

	Model* board = nullptr;
	Object3d* boardObject = nullptr;

	Stage stage;

	TitleScene* titleScene = nullptr;
	GameScene* gameScene = nullptr;

	bool isSprite = false;
	bool isModel = false;

	bool endRequest_ = false;


	enum class SpriteMoveState {
		Idle,
		Entering,
		Staying,
		Exiting
	};

	SpriteMoveState spriteMoveState = SpriteMoveState::Idle;
	float spriteMoveTimer = 0.0f;
	const float enterDuration = 1.0f;   // 入場イージング時間（秒）
	const float stayDuration = 0.5f;    // 停止時間（秒）
	const float exitDuration = 1.0f;    // 退場イージング時間（秒）
	Vector3 topStartPos = { -1280.0f, 0.0f, -200.0f }; // 画面外左
	Vector3 topEndPos = { 1280.0f, 0.0f ,-200.0f};    // 画面外右
	Vector3 topCenterPos = { 0.0f, 0.0f ,-200.0f}; // 画面中央

	Sprite* swordTop = nullptr;
	Sprite* swordMid = nullptr;
	Sprite* swordBot = nullptr;

	const float swordMidDelay = 0.1f;
	const float swordBotDelay = 0.2f;
	float swordMidTimer = 0.0f;
	float swordBotTimer = 0.0f;
	SpriteMoveState swordMidState = SpriteMoveState::Idle;
	SpriteMoveState swordBotState = SpriteMoveState::Idle;
	Vector3 midStartPos = { -1280.0f, 240.0f, -200.0f }; // 画面外左
	Vector3 midEndPos = { 1280.0f, 240.0f ,-200.0f };    // 画面外右
	Vector3 midCenterPos = { 0.0f, 240.0f ,-200.0f }; // 画面中央

	Vector3 botStartPos = { -1280.0f, 480.0f, -200.0f }; // 画面外左
	Vector3 botEndPos = { 1280.0f, 480.0f ,-200.0f };    // 画面外右
	Vector3 botCenterPos = { 0.0f, 480.0f ,-200.0f }; // 画面中央

};

