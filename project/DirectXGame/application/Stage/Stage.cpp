#include "Stage.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize(Object3dCommon* object3dCommon)
{
	stage = new Model();
	ModelManager::GetInstance()->LoadModel("stage.gltf");
	for (int y = 0; y < stageHeight; ++y) {
		for (int x = 0; x < stageWidth; ++x) {
			Object3d* block = new Object3d();
			block->Initialize(object3dCommon, stage);
			block->SetModel("stage.gltf");
			block->GetTransform().translate = { x * blockSize + stageOrigin.x, 0.0f, y * blockSize + stageOrigin.y };
			stageObjects.push_back(block);
		}
	}
}

void Stage::Finalize()
{
	for (auto block : stageObjects) {
		delete block;
	}
	stageObjects.clear();
}

void Stage::Update()
{
	for (auto block : stageObjects) {
		block->Update();
	}
}

void Stage::Draw()
{
	for (auto block : stageObjects) {
		block->Draw();
	}
}
