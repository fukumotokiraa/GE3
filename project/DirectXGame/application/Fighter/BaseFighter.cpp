#include "BaseFighter.h"

void BaseFighter::ChangeState(std::unique_ptr<FighterState> newState)
{
	if (state_) state_->Exit(this);
	state_ = std::move(newState);
	if (state_) state_->Enter(this);
}

void BaseFighter::UpdateState(float dt)
{
	if (state_) state_->Update(this, dt);
}

void BaseFighter::Update()
{
	// デフォルト：状態更新のみ行う。派生が必要に応じてオーバーライドする。
	UpdateState(1.0f / 60.0f); // 実際はフレームの dt を渡す
}
