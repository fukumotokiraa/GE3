#pragma once
#include "BaseFighter.h"

class Knight : public BaseFighter
{
public:
	~Knight() override = default;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	Object3d* GetKnightObject() { return knightObject; }

private:
	Model* knightModel = nullptr;
	Object3d* knightObject = nullptr;

};

