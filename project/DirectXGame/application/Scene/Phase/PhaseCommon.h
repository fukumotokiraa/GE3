#pragma once
#include "Model.h"
#include "ModelManager.h"
#include "Sprite.h"
#include "Input.h"
#include "Object3dCommon.h"
#include "Fighter/Knight.h"

class PhaseCommon
{
public:
	void Initialize(Object3dCommon* object3dCommon, Input* input);
	void Finalize();
	void Update();
	void Draw();

	Input* GetInput() { return input_; }
	Knight* GetKnight() { return knight_; }
	Knight* GetEnemyKnight() { return knightEnemy_; }

	//Object3dCommon* GetObject3DCommon() { return object3dCommon_; }
	//void SetObject3DCommon(Object3dCommon* object3dCommon) { object3dCommon_ = object3dCommon; }

private:
	Object3dCommon* object3dCommon_ = nullptr;

	Knight* knight_ = nullptr;
	Knight* knightEnemy_ = nullptr;
	Input* input_ = nullptr;

};

