#pragma once
#include "BaseFighter.h"
#include "Object3d.h"
#include "Model.h"

class Guardian : public BaseFighter
{
public:
	~Guardian() override = default;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void OnHit() override;

	Status* GetStatus() override { return &guardianStatus; }
	Object3d* GetObject3d() override { return guardianObject; }


private:
	Model* guardianModel = nullptr;
	Object3d* guardianObject = nullptr;
	Status guardianStatus;
};

