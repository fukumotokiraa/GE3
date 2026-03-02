#pragma once

#include <algorithm>
#include "Object3d.h"
#include "Model.h"
#include "Status.h"

class BaseFighter
{
public:
	virtual ~BaseFighter() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	// 被弾時のリアクション（デフォルトは何もしない）
	virtual void OnHit() {}

	virtual Status* GetStatus() = 0;
	virtual Object3d* GetObject3d() { return nullptr; }

	void SetObject3dCommon(Object3dCommon* object3dCommon) { object3dCommon_ = object3dCommon; }

	void SetGridPos(int x, int y) { stagePos_.x = x; stagePos_.y = y; }
	StagePos GetGridPos() const { return stagePos_; }

	// 追加: 外部から faction を設定する簡易ヘルパ
	void SetFaction(Faction f) {
		Status* s = GetStatus();
		if (s) s->faction = f;
	}

	// --- ターゲットロック関連 ---
	void SetCurrentTarget(BaseFighter* t) { currentTarget_ = t; }
	BaseFighter* GetCurrentTarget() const { return currentTarget_; }
	void ClearCurrentTarget() { currentTarget_ = nullptr; }

	// --- 攻撃関連（共通実装） ---
	// フレームごとに呼ぶ（BattlePhase 等から）
	void UpdateAttackTimer(float dt) {
		attackTimer_ -= dt;
		if (attackTimer_ < 0.0f) attackTimer_ = 0.0f;
	}

	// 攻撃可能か（タイマーとステータスがあるか）
	bool CanAttack() {
		const Status* s = GetStatus();
		if (!s) return false;
		const float atkSp = max(0.0001f, s->attackSpeed);
		// attackSpeed が高いほど短いクール（1/attackSpeed）
		return attackTimer_ <= 0.0f;
	}

	// 攻撃を行う（デフォルトのダメージ適用）
	// target の HP を減らし、自身のクールダウンをリセットする
	virtual void Attack(BaseFighter* target) {
		if (!target) return;
		Status* my = GetStatus();
		Status* t = target->GetStatus();
		if (!my || !t) return;

		t->hp = max(0, t->hp - my->power);
		// 被弾リアクションを通知
		target->OnHit();
		// クールダウンのリセット
		ResetAttackTimer();
	}

	// タイマーを攻撃速度からリセット
	void ResetAttackTimer() {
		const Status* s = GetStatus();
		const float atkSp = (s && s->attackSpeed > 0.0f) ? s->attackSpeed : 1.0f;
		attackTimer_ = 1.0f / atkSp;
	}

protected:
	Object3dCommon* object3dCommon_ = nullptr;

private:
	StagePos stagePos_ = { 0,0 };

	// 攻撃クールダウン管理（秒）
	float attackTimer_ = 0.0f;

	// 現在ロックしているターゲット（nullptr 可）
	BaseFighter* currentTarget_ = nullptr;
};

