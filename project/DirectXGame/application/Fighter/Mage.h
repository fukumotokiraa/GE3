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

	Status* GetStatus() override { return &mageStatus; }
	Object3d* GetObject3d() override { return mageObject; }
	

private:
	Model* mageModel = nullptr;
	Object3d* mageObject = nullptr;
	Status mageStatus;
};

