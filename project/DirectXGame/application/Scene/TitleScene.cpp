#include "TitleScene.h"
#include "Framework.h"
#include "GameScene.h"

void TitleScene::Initialize()
{
	titleSprite = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/Title.png");
	titleSprite->Initialize(spriteCommon_, "resources/Title.png");
	titleSprite->SetPosition({ 0.0f,0.0f });
	

}

void TitleScene::Finalize()
{
	delete titleSprite;
}

void TitleScene::Update()
{
	titleSprite->Update();
	
	if (input_->TriggerKey(DIK_SPACE)) {
		BaseScene* scene = new GameScene();
		scene->SetObject3dCommon(object3dCommon_);
		sceneManager_->SetNextScene(scene);
	}
}

void TitleScene::Draw()
{
	titleSprite->Draw();
}
