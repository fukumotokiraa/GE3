#pragma once
#include "Object3d.h"
#include "Model.h"

class BaseFighter
{
public:
	virtual ~BaseFighter() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetObject3dCommon(Object3dCommon* object3dCommon) { object3dCommon_ = object3dCommon; }

protected:
	Object3dCommon* object3dCommon_ = nullptr;

};

