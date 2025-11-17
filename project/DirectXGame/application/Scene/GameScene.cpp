#include "GameScene.h"
#include "Fighter/Knight.h"

void GameScene::Initialize()
{
	stage_ = new Stage();
	stage_->Initialize(object3dCommon_);
	preGameScene_ = new PreGameScene();
	preGameScene_->Initialize(spriteCommon_);

	//loseSprite_ = new Sprite();
	//TextureManager::GetInstance()->LoadTexture("resources/lose.png");
	//loseSprite_->Initialize(spriteCommon_, "resources/lose.png");
	//loseSprite_->SetPosition({ 0.0f,0.0f, 100.0f });

	gameover_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/gameover.png");
	gameover_->Initialize(spriteCommon_, "resources/gameover.png");

	loseSprite_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/lose.png");
	loseSprite_->Initialize(spriteCommon_, "resources/lose.png");
	loseSprite_->SetPosition(startPos_);
	loseSpriteState_ = LoseSpriteState::Idle;
	loseSpriteTimer_ = 0.0f;
	isLose_ = false;

	phaseCommon_ = new PhaseCommon();
    phaseCommon_->Initialize(object3dCommon_, input_);
	setPhase_ = new SetPhase();
	setPhase_->Initialize(phaseCommon_);

}

void GameScene::Finalize()
{
	setPhase_->Finalize();
	delete setPhase_;
	phaseCommon_->Finalize();
	delete phaseCommon_;

	delete loseSprite_;

	delete gameover_;

	stage_->Finalize();
	delete stage_;
	preGameScene_->Finalize();
	delete preGameScene_;
}

void GameScene::Update()
{
#pragma region シーン遷移
	gameover_->Update();
	loseSprite_->Update();
    if (input_->TriggerKey(DIK_RETURN)) {
        if (loseSpriteState_ == LoseSpriteState::Idle) {
            loseSpriteState_ = LoseSpriteState::Entering;
            loseSpriteTimer_ = 0.0f;
            isLose_ = true;
        }
    }
    // スペースキーで退場開始
    if (input_->TriggerKey(DIK_SPACE)) {
        if (loseSpriteState_ == LoseSpriteState::Staying) {
            loseSpriteState_ = LoseSpriteState::Exiting;
            loseSpriteTimer_ = 0.0f;
        }
    }

    // スプライトアニメーション
    switch (loseSpriteState_) {
    case LoseSpriteState::Idle:
        loseSprite_->SetPosition(startPos_);
        isLose_ = false;
        break;
    case LoseSpriteState::Entering: {
        loseSpriteTimer_ += 1.0f / 60.0f;
        float t = std::min(loseSpriteTimer_ / enterDuration_, 1.0f);
        float eased = easeOutBounce(t);
        Vector3 pos;
        pos.x = startPos_.x + (centerPos_.x - startPos_.x) * eased;
        pos.y = startPos_.y + (centerPos_.y - startPos_.y) * eased;
        pos.z = startPos_.z;
        loseSprite_->SetPosition(pos);
        if (t >= 1.0f) {
            loseSpriteState_ = LoseSpriteState::Staying;
            loseSpriteTimer_ = 0.0f;
        }
        break;
    }
    case LoseSpriteState::Staying:
        loseSprite_->SetPosition(centerPos_);
        loseSpriteTimer_ += 1.0f / 60.0f;
        if (loseSpriteTimer_ >= stayDuration_) {
            // 何もしない（エンターキーでExitingへ）
        }
        break;
    case LoseSpriteState::Exiting: {
        loseSpriteTimer_ += 1.0f / 60.0f;
        float t = std::min(loseSpriteTimer_ / exitDuration_, 1.0f);
        float eased = easeInBack(t);
        Vector3 pos;
        pos.x = centerPos_.x + (endPos_.x - centerPos_.x) * eased;
        pos.y = centerPos_.y + (endPos_.y - centerPos_.y) * eased;
        pos.z = centerPos_.z;
        loseSprite_->SetPosition(pos);
        if (t >= 1.0f) {
            loseSpriteState_ = LoseSpriteState::Idle;
            loseSpriteTimer_ = 0.0f;
            isLose_ = false;
        }
        break;
    }
    }
#pragma endregion



	stage_->Update();
	preGameScene_->Update();

	phaseCommon_->Update();
	setPhase_->Update();

}

void GameScene::Draw()
{
/*	if (isLose_ == true) {
loseSprite_->Draw();
	}	*/	
	stage_->Draw();
	//gameover_->Draw();
	preGameScene_->Draw();
    if (isLose_ && loseSprite_) {
        loseSprite_->Draw();
    }

	phaseCommon_->Draw();
	setPhase_->Draw();
}
