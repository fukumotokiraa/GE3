#include "GameScene.h"

void GameScene::Initialize()
{
	stage_ = new Stage();
	stage_->Initialize(object3dCommon_);
	preGameScene_ = new PreGameScene();
	preGameScene_->Initialize(spriteCommon_);

}

void GameScene::Finalize()
{
	stage_->Finalize();
	delete stage_;
	preGameScene_->Finalize();
	delete preGameScene_;
}

void GameScene::Update()
{
	stage_->Update();
	preGameScene_->Update();
}

void GameScene::Draw()
{
	stage_->Draw();
	preGameScene_->Draw();
}
