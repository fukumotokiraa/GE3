#pragma once
#include "BaseFighter.h"
#include "Object3d.h"
#include "Model.h"

class Mage : public BaseFighter
{
	public:
	~Mage() override = default;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void OnHit() override;

	// Mage 固有の攻撃処理（弾を飛ばす）
	void Attack(BaseFighter* target) override;

	Status* GetStatus() override { return &mageStatus; }
	Object3d* GetObject3d() override { return mageObject; }
	

private:
	Model* mageModel = nullptr;
	Object3d* mageObject = nullptr;
	Status mageStatus;

	// 弾関連
	Model* bulletModel = nullptr;
	Object3d* bulletObject = nullptr;
	bool bulletActive = false;
	Vector3 bulletStart{};
	Vector3 bulletEnd{};
	float bulletElapsed = 0.0f;
	float bulletDuration = 0.25f; // 初期値（距離に応じて計算）
	BaseFighter* bulletTarget = nullptr;
};

