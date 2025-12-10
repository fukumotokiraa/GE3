#include "Knight.h"
#include "ModelManager.h"

void Knight::Initialize()
{
	ModelManager::GetInstance()->LoadModel("knightTest.gltf");
	knightModel = new Model();
	knightObject = new Object3d();
	knightObject->Initialize(object3dCommon_, knightModel);
	knightObject->SetModel("knightTest.gltf");
	knightObject->GetTransform().translate = { -4.0f,1.0f,-4.0f };

	knightStatus = {
		Faction::Player,
		100,
		100,
		1, // moveRange（移動は現状 1 固定）
		1, // range（攻撃距離: 隣接）
		20 // attack
	};
}

void Knight::Finalize()
{
	delete knightObject;
	delete knightModel;
}

void Knight::Update()
{
	if (knightObject) knightObject->Update();
}

void Knight::Draw()
{
	if (knightObject) knightObject->Draw();
}

void Knight::OnHit()
{
	// 点滅の長さと間隔はここで調整
	if (knightObject) {
		knightObject->StartBlink(0.6f, 0.08f); // 0.6秒、0.08秒間隔で点滅
	}
}
