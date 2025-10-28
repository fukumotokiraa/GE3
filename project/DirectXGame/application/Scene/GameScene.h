#pragma once
#include "BaseScene.h"
#include "PreGameScene.h"
#include "Stage.h"

class PreGameScene;
class Stage;


class GameScene : public BaseScene
{
public:
	~GameScene()override = default;
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

	PreGameScene* GetPreGameScene() { return preGameScene_; }
	Stage* GetStage() { return stage_; }

	float easeOutBounce(float x) {
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (x < 1.0f / d1) {
			return n1 * x * x;
		}
		else if (x < 2.0f / d1) {
			return n1 * (x -= 1.5f / d1) * x + 0.75f;
		}
		else if (x < 2.5f / d1) {
			return n1 * (x -= 2.25f / d1) * x + 0.9375f;
		}
		else {
			return n1 * (x -= 2.625f / d1) * x + 0.984375f;
		}
	}

	float easeInBack(float x) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;

		return c3 * x * x * x - c1 * x * x;
	}

private:
	Stage* stage_ = nullptr;
	PreGameScene* preGameScene_ = nullptr;

	Sprite* gameover_ = nullptr;
	Sprite* loseSprite_ = nullptr;
	bool isLose_ = false;

	enum class LoseSpriteState {
		Idle,
		Entering,
		Staying,
		Exiting
	};
	LoseSpriteState loseSpriteState_ = LoseSpriteState::Idle;
	float loseSpriteTimer_ = 0.0f;
	const float enterDuration_ = 1.0f;
	const float stayDuration_ = 0.5f;
	const float exitDuration_ = 1.0f;

	// スプライトの移動座標
	Vector3 startPos_ = { 0.0f, -720.0f, 0.0f };   // 画面外上
	Vector3 centerPos_ = { 0.0f, 0.0f, 0.0f };   // 画面中央
	Vector3 endPos_ = { 0.0f, 720.0f, 0.0f };
};

