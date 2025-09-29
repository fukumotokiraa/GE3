#pragma once
#include "Calculation.h"
#include "Model.h"
#include "Object3d.h"

class Stage
{
public:
	Stage();
	~Stage();
	void Initialize(Object3dCommon* object3dCommon);
	void Finalize();
	void Update();
	void Draw();

private:
	Model* stage = nullptr;
	std::vector<Object3d*> stageObjects;
	Vector3 stageOrigin = { -4.0f,-4.0f,0.0f };
	const int stageWidth = 8;
	const int stageHeight = 8;
	const float blockSize = 1.0f;

};

