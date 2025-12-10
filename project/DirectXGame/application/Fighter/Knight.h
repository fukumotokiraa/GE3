#pragma once
#include "BaseFighter.h"
#include "Object3d.h"
#include "Model.h"

class Knight : public BaseFighter
{
public:
	~Knight() override = default;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	Object3d* GetKnightObject() { return knightObject; }

	Status* GetStatus() override { return &knightStatus; }

	// 被弾時のリアクションをオーバーライド
	void OnHit() override;

private:
	Model* knightModel = nullptr;
	Object3d* knightObject = nullptr;
	Status knightStatus;
};

