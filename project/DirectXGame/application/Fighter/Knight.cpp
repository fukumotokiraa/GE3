#include "Knight.h"
#include "ModelManager.h"

void Knight::Initialize()
{
	ModelManager::GetInstance()->LoadModel("knightTest.gltf");
	// 陣営に応じて使用モデルを切り替える
	const Status* st = GetStatus();
	const bool isEnemy = (st && st->faction == Faction::Enemy);

	const std::string modelFile = isEnemy ? "knightEnemy.gltf" : "knightTest.gltf";

	ModelManager::GetInstance()->LoadModel(modelFile);
	knightModel = new Model();
	knightObject = new Object3d();
	knightObject->Initialize(object3dCommon_, knightModel);
	knightObject->SetModel(modelFile);
	knightObject->GetTransform().translate = { -4.0f,1.0f,-4.0f };

	// SpawnFighter で faction が既に設定されている前提のため、ここでは上書きしない
	knightStatus.hp = 100;
	knightStatus.maxHp = 100;
	knightStatus.move = 1; // moveRange（移動は現状 1 固定）
	knightStatus.range = 1; // range（攻撃距離: 隣接）
	knightStatus.power = 20; // attack
	knightStatus.attackSpeed = 1.0f; // attackSpeed
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
