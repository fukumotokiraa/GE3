#include "Mage.h"
#include "ModelManager.h"

void Mage::Initialize()
{
	// 陣営に応じて使用モデルを切り替える
	const Status* st = GetStatus();
	const bool isEnemy = (st && st->faction == Faction::Enemy);

	const std::string modelFile = isEnemy ? "mageEnemy.gltf" : "mage.gltf";

	// モデル読み込みと Object3d をセットアップ
	ModelManager::GetInstance()->LoadModel(modelFile);
	mageModel = new Model();
	mageObject = new Object3d();
	mageObject->Initialize(object3dCommon_, mageModel);
	mageObject->SetModel(modelFile);
	mageObject->GetTransform().translate = { 3.0f,1.0f,-4.0f };

	// 注意: SpawnFighter で既に faction をセットしている想定なので、
	//       ここでは faction を上書きしないように他フィールドのみ代入する
	mageStatus.hp = 80;
	mageStatus.maxHp = 80;
	mageStatus.move = 1; // moveRange
	mageStatus.range = 4; // range
	mageStatus.power = 10; // attack
	mageStatus.attackSpeed = 0.6f; // attackSpeed
}

void Mage::Finalize()
{
	delete mageObject;
	delete mageModel;
}

void Mage::Update()
{
	if (mageObject) mageObject->Update();
}

void Mage::Draw()
{
	if (mageObject) mageObject->Draw();
}

void Mage::OnHit()
{
	// 点滅の長さと間隔はここで調整
	if (mageObject) {
		mageObject->StartBlink(0.6f, 0.08f); // 0.6秒、0.08秒間隔で点滅
	}
}
