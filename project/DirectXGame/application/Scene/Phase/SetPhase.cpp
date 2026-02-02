#include "SetPhase.h"
#include "PhaseCommon.h"
#include "Fighter/Mage.h"
#include "Fighter/BaseFighter.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Model.h"
#include <algorithm>

void SetPhase::Initialize(PhaseCommon* phaseCommon)
{
	phaseCommon_ = phaseCommon;

	// フレーム / 選択フレームのモデルを読み込み
	ModelManager::GetInstance()->LoadModel("frame.gltf");
	ModelManager::GetInstance()->LoadModel("dFrame.gltf");

	// Object3d を生成（既存プロジェクトの流儀に合わせる）
	frameModel_ = new Model();
	dframeModel_ = new Model();
	frameObject_ = new Object3d();
	if (phaseCommon_) {
		frameObject_->Initialize(phaseCommon_->GetObject3dCommon(), frameModel_);
		frameObject_->SetModel("frame.gltf");
		// カーソル初期位置：プレイヤー Mage がいればその位置に置く
		Mage* playerMage = phaseCommon_->GetFirstOfType<Mage>(Team::Player);
		if (playerMage) {
			auto gp = playerMage->GetGridPos();
			cursorX_ = std::clamp(gp.x, 0, kStageWidth_ - 1);
			cursorY_ = std::clamp(gp.y, 0, kStageHeight_ - 1);
		}
		float wx = kOriginX_ + cursorX_ * kBlockSize_;
		float wz = kOriginY_ + cursorY_ * kBlockSize_;
		frameObject_->GetTransform().translate = { wx, kFrameHeight_, wz };
	}
}

void SetPhase::Finalize()
{
	if (frameObject_) {
		delete frameObject_;
		frameObject_ = nullptr;
	}
	if (frameModel_) {
		delete frameModel_;
		frameModel_ = nullptr;
	}
	if (dframeModel_) {
		delete dframeModel_;
		dframeModel_ = nullptr;
	}
}

void SetPhase::Update()
{
    if (!phaseCommon_) return;

    // 入力取得
    Input* input = phaseCommon_->GetInput();
    if (!input) return;

    // WASD でカーソル移動（1 押下につき 1 マス）
    if (input->TriggerKey(DIK_W)) {
        cursorY_ = std::clamp(cursorY_ + 1, 0, kStageHeight_ - 1);
    }
    if (input->TriggerKey(DIK_S)) {
        cursorY_ = std::clamp(cursorY_ - 1, 0, kStageHeight_ - 1);
    }
    if (input->TriggerKey(DIK_A)) {
        cursorX_ = std::clamp(cursorX_ - 1, 0, kStageWidth_ - 1);
    }
    if (input->TriggerKey(DIK_D)) {
        cursorX_ = std::clamp(cursorX_ + 1, 0, kStageWidth_ - 1);
    }

    // Space トグル: 未選択 -> 味方がいる場所なら選択（dFrame に切替）、選択中 -> 確定（frame に戻す）
    if (input->TriggerKey(DIK_SPACE)) {
        if (!isHolding_) {
            // カーソル上にいる味方を検索（プレイヤーチーム）
            auto players = phaseCommon_->GetAllOfType<BaseFighter>(Team::Player);
            for (auto p : players) {
                if (!p) continue;
                auto gp = p->GetGridPos();
                if (gp.x == cursorX_ && gp.y == cursorY_) {
                    selectedFighter_ = p;
                    break;
                }
            }
            if (selectedFighter_) {
                isHolding_ = true;
                // フレームを選択用に切替
                frameObject_->SetModel("dFrame.gltf");
            }
        } else {
            // 確定: 選択ユニットを現在のカーソル位置に配置して解放
            if (selectedFighter_) {
                selectedFighter_->SetGridPos(cursorX_, cursorY_);
                if (auto obj = selectedFighter_->GetObject3d()) {
                    float wx = kOriginX_ + cursorX_ * kBlockSize_;
                    float wz = kOriginY_ + cursorY_ * kBlockSize_;
                    obj->GetTransform().translate = { wx, kUnitWorldHeight_, wz };
                }
            }
            selectedFighter_ = nullptr;
            isHolding_ = false;
            frameObject_->SetModel("frame.gltf");
        }
    }

    // フレームオブジェクト更新（常にカーソル位置にスナップ）
    if (frameObject_) {
        float wx = kOriginX_ + cursorX_ * kBlockSize_;
        float wz = kOriginY_ + cursorY_ * kBlockSize_;
        frameObject_->GetTransform().translate = { wx, kFrameHeight_, wz };
        frameObject_->Update();
    }

    // 選択中なら選択ユニットをカーソル位置に追従させる（視覚的に持ち上げる）
    if (isHolding_ && selectedFighter_) {
        if (auto obj = selectedFighter_->GetObject3d()) {
            float wx = kOriginX_ + cursorX_ * kBlockSize_;
            float wz = kOriginY_ + cursorY_ * kBlockSize_;
            obj->GetTransform().translate = { wx, kUnitWorldHeight_, wz };
        }
    }

    // プレイヤー Mage が居る場合は内部 GridPos を更新（従来の処理継続）
    Mage* playerMage = phaseCommon_->GetFirstOfType<Mage>(Team::Player);
    if (playerMage) {
        // 既存のロジック： Object3d の位置からグリッド算出して SetGridPos（ただし選択中は上書きしない）
        Object3d* obj = playerMage->GetObject3d();
        if (obj && !isHolding_) {
            int gx = static_cast<int>(obj->GetTransform().translate.x + 4.0f + 0.5f);
            int gy = static_cast<int>(obj->GetTransform().translate.z + 4.0f + 0.5f);
            gx = std::clamp(gx, 0, kStageWidth_ - 1);
            gy = std::clamp(gy, 0, kStageHeight_ - 1);
            playerMage->SetGridPos(gx, gy);
        }
    }
}

void SetPhase::Draw()
{
	// フレームは SetPhase の描画タイミングで描画
	if (frameObject_) frameObject_->Draw();
}
