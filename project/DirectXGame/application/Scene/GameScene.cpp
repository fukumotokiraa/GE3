#include "GameScene.h"
#include "Fighter/Knight.h"
#include "Fighter/FighterFactory.h"
#include "Fighter/BaseFighter.h"
#include "TitleScene.h"

void GameScene::Initialize()
{
	stage_ = new Stage();
	stage_->Initialize(object3dCommon_);
	preGameScene_ = new PreGameScene();
	preGameScene_->Initialize(spriteCommon_);
	//clear_ = new Clear();
	//clear_->Initialize(spriteCommon_);

	// loseSprite を初期化（clearSprite と同様の挙動）
	loseSprite_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/lose.png");
	loseSprite_->Initialize(spriteCommon_, "resources/lose.png");
	loseSprite_->SetPosition(startPos_);
	loseSpriteState_ = clearSpriteState::Idle;
	loseSpriteTimer_ = 0.0f;
	islose_ = false;

	gameover_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/start.png");
	gameover_->Initialize(spriteCommon_, "resources/start.png");
	gameover_->SetPosition({ 0.0f, 0.0f, 20.0f });

	operation_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/operation.png");
	operation_->Initialize(spriteCommon_, "resources/operation.png");
	operation_->SetPosition({ 300.0f, 520.0f, 20.0f });

	poseSprite_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/pose.png");
	poseSprite_->Initialize(spriteCommon_, "resources/pose.png");
	poseSprite_->SetPosition({ 450.0f, 200.0f, 0.0f });

	clearSprite_ = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/clear.png");
	clearSprite_->Initialize(spriteCommon_, "resources/clear.png");
	clearSprite_->SetPosition(startPos_);
	clearSpriteState_ = clearSpriteState::Idle;
	clearSpriteTimer_ = 0.0f;
	isclear_ = false;

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

	if (auto eGuardian = phaseCommon_->SpawnFighter(FighterType::Guardian, Team::Enemy)) {
		eGuardian->SetGridPos(0, 7);
		if (auto obj = eGuardian->GetObject3d()) {
			obj->GetTransform().translate = { kOriginX + eGuardian->GetGridPos().x * kBlockSize, kWorldHeight, kOriginY + eGuardian->GetGridPos().y * kBlockSize };
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

	delete clearSprite_;
	delete loseSprite_;
	delete poseSprite_;
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
	clearSprite_->Update();
	loseSprite_->Update();
	poseSprite_->Update();
	//clear_->Update();
    if (input_->TriggerKey(DIK_RETURN)) {
        if (clearSpriteState_ == clearSpriteState::Idle) {
            clearSpriteState_ = clearSpriteState::Entering;
            clearSpriteTimer_ = 0.0f;
            isclear_ = true;
			//clear_->SetClear(true);
        }
    }
    // スペースキーで退場開始
    if (input_->TriggerKey(DIK_SPACE)) {
        if (clearSpriteState_ == clearSpriteState::Staying) {
            clearSpriteState_ = clearSpriteState::Exiting;
            clearSpriteTimer_ = 0.0f;
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
        }
        if (loseSpriteState_ == clearSpriteState::Staying) {
            loseSpriteState_ = clearSpriteState::Exiting;
            loseSpriteTimer_ = 0.0f;
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
        }
    }

	// タイトルへ戻る（Tキー）
	if (input_->TriggerKey(DIK_T)) {
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
		return;
	}

	if (input_->TriggerKey(DIK_ESCAPE)) {
		if(isPose_ == false) {
			isPose_ = true;
		}
		else {
			isPose_ = false;
		}
	}

    // スプライトアニメーション（clear）
    switch (clearSpriteState_) {
    case clearSpriteState::Idle:
        clearSprite_->SetPosition(startPos_);
        isclear_ = false;
        break;
    case clearSpriteState::Entering: {
        clearSpriteTimer_ += 1.0f / 60.0f;
        float t = std::min(clearSpriteTimer_ / enterDuration_, 1.0f);
        float eased = easeOutBounce(t);
        Vector3 pos;
        pos.x = startPos_.x + (centerPos_.x - startPos_.x) * eased;
        pos.y = startPos_.y + (centerPos_.y - startPos_.y) * eased;
        pos.z = startPos_.z;
        clearSprite_->SetPosition(pos);
        if (t >= 1.0f) {
            clearSpriteState_ = clearSpriteState::Staying;
            clearSpriteTimer_ = 0.0f;
        }
        break;
    }
    case clearSpriteState::Staying:
        clearSprite_->SetPosition(centerPos_);
        clearSpriteTimer_ += 1.0f / 60.0f;
        if (clearSpriteTimer_ >= stayDuration_) {
            // 何もしない（エンターキーでExitingへ）
        }
        break;
    case clearSpriteState::Exiting: {
        clearSpriteTimer_ += 1.0f / 60.0f;
        float t = std::min(clearSpriteTimer_ / exitDuration_, 1.0f);
        float eased = easeInBack(t);
        Vector3 pos;
        pos.x = centerPos_.x + (endPos_.x - centerPos_.x) * eased;
        pos.y = centerPos_.y + (endPos_.y - centerPos_.y) * eased;
        pos.z = centerPos_.z;
        clearSprite_->SetPosition(pos);
        if (t >= 1.0f) {
            clearSpriteState_ = clearSpriteState::Idle;
            clearSpriteTimer_ = 0.0f;
            isclear_ = false;
        }
        break;
    }
    }

	// スプライトアニメーション（lose - clear と同様の挙動）
	switch (loseSpriteState_) {
	case clearSpriteState::Idle:
		loseSprite_->SetPosition(startPos_);
		islose_ = false;
		break;
	case clearSpriteState::Entering: {
		loseSpriteTimer_ += 1.0f / 60.0f;
		float t = std::min(loseSpriteTimer_ / enterDuration_, 1.0f);
		float eased = easeOutBounce(t);
		Vector3 pos;
		pos.x = startPos_.x + (centerPos_.x - startPos_.x) * eased;
		pos.y = startPos_.y + (centerPos_.y - startPos_.y) * eased;
		pos.z = startPos_.z;
		loseSprite_->SetPosition(pos);
		if (t >= 1.0f) {
			loseSpriteState_ = clearSpriteState::Staying;
			loseSpriteTimer_ = 0.0f;
		}
		break;
	}
	case clearSpriteState::Staying:
		loseSprite_->SetPosition(centerPos_);
		loseSpriteTimer_ += 1.0f / 60.0f;
		if (loseSpriteTimer_ >= stayDuration_) {
			// 何もしない（スペースキーでExitingへ、clear と同様）
		}
		break;
	case clearSpriteState::Exiting: {
		loseSpriteTimer_ += 1.0f / 60.0f;
		float t = std::min(loseSpriteTimer_ / exitDuration_, 1.0f);
		float eased = easeInBack(t);
		Vector3 pos;
		pos.x = centerPos_.x + (endPos_.x - centerPos_.x) * eased;
		pos.y = centerPos_.y + (endPos_.y - centerPos_.y) * eased;
		pos.z = centerPos_.z;
		loseSprite_->SetPosition(pos);
		if (t >= 1.0f) {
			loseSpriteState_ = clearSpriteState::Idle;
			loseSpriteTimer_ = 0.0f;
			islose_ = false;
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

	// battlePhase 中に敵が全員いなくなったらスプライト表示を開始する（既存）
	if (battlePhase_ && phaseCommon_) {
		// PhaseCommon::Update() ですでに死亡したファイターは除去されているため
		auto enemies = phaseCommon_->GetAllOfType<BaseFighter>(Team::Enemy);
		bool anyEnemyAlive = false;
		for (auto e : enemies) {
			if (e && e->GetStatus() && e->GetStatus()->IsAlive()) { anyEnemyAlive = true; break; }
		}
		if (!anyEnemyAlive) {
			if (clearSpriteState_ == clearSpriteState::Idle) {
				clearSpriteState_ = clearSpriteState::Entering;
				clearSpriteTimer_ = 0.0f;
				isclear_ = true;
				//clear_->SetClear(true);
			}
		}
		// 逆にプレイヤー側が全員いなくなったら lose を表示する
		auto players = phaseCommon_->GetAllOfType<BaseFighter>(Team::Player);
		bool anyPlayerAlive = false;
		for (auto p : players) {
			if (p && p->GetStatus() && p->GetStatus()->IsAlive()) { anyPlayerAlive = true; break; }
		}
		if (!anyPlayerAlive) {
			if (loseSpriteState_ == clearSpriteState::Idle) {
				loseSpriteState_ = clearSpriteState::Entering;
				loseSpriteTimer_ = 0.0f;
				islose_ = true;
			}
		}
	}

}

void GameScene::Draw()
{
	// 先に勝利／敗北のスプライトを描画（どちらかが起動していれば中央に表示）
	if (isclear_ && clearSprite_) {
        clearSprite_->Draw();
    }
	if (islose_ && loseSprite_) {
		loseSprite_->Draw();
	}

	//if (clear_->GetClear()) {
	//	gameover_->Draw();
	//	return;
	//}
	stage_->Draw();
	gameover_->Draw();
	operation_->Draw();
	if (isPose_ == true) {
		poseSprite_->Draw();
	}
	preGameScene_->Draw();

	phaseCommon_->Draw();
	if (setPhase_) setPhase_->Draw();
	if (battlePhase_) battlePhase_->Draw();
}
