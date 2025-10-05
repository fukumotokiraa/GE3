#include "GameScene.h"

void GameScene::Initialize()
{
	stage.Initialize(object3dCommon_);
}

void GameScene::Finalize()
{
	stage.Finalize();
}

void GameScene::Update()
{
	stage.Update();
}

void GameScene::Draw()
{
	stage.Draw();
}
