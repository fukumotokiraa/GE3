#include "PhaseCommon.h"

static inline Vector3 GridToWorld(int gx, int gy)
{
	return { -4.0f + static_cast<float>(gx) * 1.0f, 1.0f, -4.0f + static_cast<float>(gy) * 1.0f };
}

void PhaseCommon::Initialize(Object3dCommon* object3dCommon, Input* input)
{
	object3dCommon_ = object3dCommon;
	input_ = input;

	// Player Knight の生成
	knight_ = new Knight();
	knight_->SetObject3dCommon(object3dCommon_);
	knight_->Initialize();
	knight_->SetFaction(Faction::Player);
	knight_->SetGridPos(0, 0);
	if (knight_->GetKnightObject()) {
		knight_->GetKnightObject()->GetTransform().translate = GridToWorld(knight_->GetGridPos().x,knight_->GetGridPos().y);
	}

	// Enemy Knight の生成（テスト用） — グリッド (3,3) に配置
	knightEnemy_ = new Knight();
	knightEnemy_->SetObject3dCommon(object3dCommon_);
	knightEnemy_->Initialize();
	knightEnemy_->SetFaction(Faction::Enemy);
	knightEnemy_->SetGridPos(7, 7); // <- ここを (3,3) に変更
	if (knightEnemy_->GetKnightObject()) {
		knightEnemy_->GetKnightObject()->GetTransform().translate = GridToWorld(knightEnemy_->GetGridPos().x,knightEnemy_->GetGridPos().y);
	}
}

void PhaseCommon::Finalize()
{
	delete knight_;
	delete knightEnemy_;
}

void PhaseCommon::Update()
{
	if (knight_) knight_->Update();
	if (knightEnemy_) knightEnemy_->Update();
}

void PhaseCommon::Draw()
{
	if (knight_) knight_->Draw();
	if (knightEnemy_) knightEnemy_->Draw();
}
