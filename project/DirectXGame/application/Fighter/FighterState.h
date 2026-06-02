#pragma once
#include <memory>

class BaseFighter; // forward

// 戦闘ユニットの状態を表す抽象クラス
class FighterState
{
public:
	virtual ~FighterState() = default;

	// 状態に入るときに一度呼ばれる
	virtual void Enter(BaseFighter* owner) {}

	// 状態を抜けるときに一度呼ばれる
	virtual void Exit(BaseFighter* owner) {}

	// 毎フレーム呼ばれる更新処理（dt: 経過秒）
	virtual void Update(BaseFighter* owner, float dt) = 0;

	// デバッグ用の名前
	virtual const char* Name() const = 0;
};

// 簡単な待機状態の例
class IdleState : public FighterState
{
public:
	void Enter(BaseFighter* owner) override;
	void Update(BaseFighter* owner, float dt) override;
	const char* Name() const override { return "Idle"; }
};

// 簡単な攻撃状態の例
class AttackState : public FighterState
{
public:
	void Enter(BaseFighter* owner) override;
	void Update(BaseFighter* owner, float dt) override;
	const char* Name() const override { return "Attack"; }
};
