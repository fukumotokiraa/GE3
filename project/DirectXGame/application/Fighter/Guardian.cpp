#include "Guardian.h"
#include "ModelManager.h"

void Guardian::Initialize()
{
	ModelManager::GetInstance()->LoadModel("guardianTest.gltf");
	// 陣営に応じて使用モデルを切り替える
	const Status* st = GetStatus();
	const bool isEnemy = (st && st->faction == Faction::Enemy);

	const std::string modelFile = isEnemy ? "guardianEnemy.gltf" : "guardianTest.gltf";

	ModelManager::GetInstance()->LoadModel(modelFile);
	guardianModel = new Model();
	guardianObject = new Object3d();
	guardianObject->Initialize(object3dCommon_, guardianModel);
	guardianObject->SetModel(modelFile);
	guardianObject->GetTransform().translate = { -4.0f,1.0f,-4.0f };

	// SpawnFighter で faction が既に設定されている前提のため、ここでは上書きしない
	guardianStatus.hp = 200;
	guardianStatus.maxHp = 200;
	guardianStatus.move = 1; // moveRange（移動は現状 1 固定）
	guardianStatus.range = 1; // range（攻撃距離: 隣接）
	guardianStatus.power = 10; // attack
	guardianStatus.attackSpeed = 0.8f; // attackSpeed
}

void Guardian::Finalize()
{
	delete guardianObject;
	delete guardianModel;
}

void Guardian::Update()
{
	if (guardianObject) guardianObject->Update();
}

void Guardian::Draw()
{
	if (guardianObject) guardianObject->Draw();
}

void Guardian::OnHit()
{
	// 点滅の長さと間隔はここで調整
	if (guardianObject) {
		guardianObject->StartBlink(0.6f, 0.08f); // 0.6秒、0.08秒間隔で点滅
	}
}
