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
	void OnHit() override;

	Status* GetStatus() override { return &knightStatus; }

	Object3d* GetKnightObject() { return knightObject; }


private:
	Model* knightModel = nullptr;
	Object3d* knightObject = nullptr;
	Status knightStatus;
};

