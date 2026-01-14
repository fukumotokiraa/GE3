#include "GameScene.h"
#include "Fighter/Knight.h"
#include "Fighter/FighterFactory.h"

void GameScene::Initialize()
{
	stage_ = new Stage();
	stage_->Initialize(object3dCommon_);
	preGameScene_ = new PreGameScene();
	preGameScene_->Initialize(spriteCommon_);
	//clear_ = new Clear();
	//clear_->Initialize(spriteCommon_);

	//loseSprite_ = new Sprite();
	//TextureManager::GetInstance()->LoadTexture("resources/lose.png");
	//loseSprite_->Initialize(spriteCommon_, "resources/lose.png");
	//loseSprite_->SetPosition({ 0.0f,0.0f, 100.0f });

	gameover_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/start.png");
	gameover_->Initialize(spriteCommon_, "resources/start.png");

	operation_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/operation.png");
	operation_->Initialize(spriteCommon_, "resources/operation.png");
	operation_->SetPosition({ 300.0f, 520.0f, 0.0f });


	loseSprite_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/clear.png");
	loseSprite_->Initialize(spriteCommon_, "resources/clear.png");
	loseSprite_->SetPosition(startPos_);
	loseSpriteState_ = LoseSpriteState::Idle;
	loseSpriteTimer_ = 0.0f;
	isLose_ = false;

	phaseCommon_ = new PhaseCommon();
    phaseCommon_->Initialize(object3dCommon_, input_);
	// ここでプレイヤー用 Mage を確実に生成しておく
	//phaseCommon_->SpawnFighter(FighterType::Mage, Team::Player);
	const float kOriginX = -4.0f;
	const float kOriginY = -4.0f;
	const float kBlockSize = 1.0f;
	const float kWorldHeight = 1.0f;
	// プレイヤー：ナイトを左端に置く（例：グリッド (0,0)）
	if (auto pKnight = phaseCommon_->SpawnFighter(FighterType::Knight, Team::Player)) {
		pKnight->SetGridPos(0, 0);
		if (auto obj = pKnight->GetObject3d()) {
			obj->GetTransform().translate = { kOriginX + pKnight->GetGridPos().x * kBlockSize, kWorldHeight, kOriginY + pKnight->GetGridPos().y * kBlockSize };
		}
	}

	// プレイヤー：メイジを右端に置く（例：グリッド (7,0)）
	if (auto pMage = phaseCommon_->SpawnFighter(FighterType::Mage, Team::Player)) {
		pMage->SetGridPos(7, 0);
		if (auto obj = pMage->GetObject3d()) {
			obj->GetTransform().translate = { kOriginX + pMage->GetGridPos().x * kBlockSize, kWorldHeight, kOriginY + pMage->GetGridPos().y * kBlockSize };
		}
	}

	// 敵：ナイトを中央付近に置く（例：グリッド (4,7)）
	if (auto eKnight = phaseCommon_->SpawnFighter(FighterType::Knight, Team::Enemy)) {
		eKnight->SetGridPos(4, 7);
		if (auto obj = eKnight->GetObject3d()) {
			obj->GetTransform().translate = { kOriginX + eKnight->GetGridPos().x * kBlockSize, kWorldHeight, kOriginY + eKnight->GetGridPos().y * kBlockSize };
		}
	}

	setPhase_ = new SetPhase();
	setPhase_->Initialize(phaseCommon_);

	// battlePhase_ は遷移まで null のまま
	battlePhase_ = nullptr;
}

void GameScene::Finalize()
{
	if (setPhase_) {
		setPhase_->Finalize();
		delete setPhase_;
		setPhase_ = nullptr;
	}
	if (battlePhase_) {
		battlePhase_->Finalize();
		delete battlePhase_;
		battlePhase_ = nullptr;
	}
	phaseCommon_->Finalize();
	delete phaseCommon_;

	delete loseSprite_;

	delete operation_;

	delete gameover_;

	//clear_->Finalize();
	//delete clear_;
	stage_->Finalize();
	delete stage_;
	preGameScene_->Finalize();
	delete preGameScene_;
}

void GameScene::Update()
{
#pragma region シーン遷移
	gameover_->Update();
	operation_->Update();
	loseSprite_->Update();
	//clear_->Update();
    if (input_->TriggerKey(DIK_RETURN)) {
        if (loseSpriteState_ == LoseSpriteState::Idle) {
            loseSpriteState_ = LoseSpriteState::Entering;
            loseSpriteTimer_ = 0.0f;
            isLose_ = true;
			//clear_->SetClear(true);
        }
    }
    // スペースキーで退場開始
    if (input_->TriggerKey(DIK_RETURN)) {
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

	if (input_->TriggerKey(DIK_Q) && setPhase_ && !battlePhase_) {
		// setPhase を終了して破棄
		setPhase_->Finalize();
		delete setPhase_;
		setPhase_ = nullptr;

		// BattlePhase を生成して初期化
		battlePhase_ = new BattlePhase();
		battlePhase_->Initialize(phaseCommon_);
	}

	// フェーズの Update を呼ぶ（存在する方）
	if (setPhase_) setPhase_->Update();
	if (battlePhase_) battlePhase_->Update();

	// battlePhase 中に敵が全員いなくなったらスプライト表示を開始する
	if (battlePhase_ && phaseCommon_) {
		// PhaseCommon::Update() ですでに死亡したファイターは除去されているため
		// GetEnemyKnight() が nullptr なら敵は存在しないと判断できる
		if (phaseCommon_->GetFirstOfType<Knight>(Team::Enemy) == nullptr) {
			if (loseSpriteState_ == LoseSpriteState::Idle) {
				loseSpriteState_ = LoseSpriteState::Entering;
				loseSpriteTimer_ = 0.0f;
				isLose_ = true;
				//clear_->SetClear(true);
			}
		}
	}

}

void GameScene::Draw()
{
	if (isLose_ == true) {
        loseSprite_->Draw();
	}		
	//if (clear_->GetClear()) {
	//	gameover_->Draw();
	//	return;
	//}
	stage_->Draw();
	gameover_->Draw();
	operation_->Draw();
	preGameScene_->Draw();
    if (isLose_ && loseSprite_) {
        loseSprite_->Draw();
    }

	phaseCommon_->Draw();
	if (setPhase_) setPhase_->Draw();
	if (battlePhase_) battlePhase_->Draw();
}
