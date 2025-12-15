#include "Mage.h"
#include "ModelManager.h"

void Mage::Initialize()
{
	ModelManager::GetInstance()->LoadModel("mage.gltf");
	mageModel = new Model();
	mageObject = new Object3d();
	mageObject->Initialize(object3dCommon_, mageModel);
	mageObject->SetModel("mage.gltf");
	mageObject->GetTransform().translate = { 3.0f,1.0f,-4.0f };
	mageStatus = {
		Faction::Player,
		80,
		80,
		1, // moveRange
		4, // range
		60, // attack
		0.6f // attackSpeed
	};
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
