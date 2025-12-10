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
	{
		auto p = std::make_unique<Knight>();
		p->SetObject3dCommon(object3dCommon_);
		p->Initialize();
		p->SetFaction(Faction::Player);
		p->SetGridPos(0, 0);
		if (auto kObj = p->GetKnightObject()) {
			kObj->GetTransform().translate = GridToWorld(p->GetGridPos().x, p->GetGridPos().y);
		}
		fighters_.push_back(std::move(p));
	}

	// Enemy Knight の生成（テスト用）
	{
		auto e = std::make_unique<Knight>();
		e->SetObject3dCommon(object3dCommon_);
		e->Initialize();
		e->SetFaction(Faction::Enemy);
		e->SetGridPos(7, 7);
		if (auto kObj = e->GetKnightObject()) {
			kObj->GetTransform().translate = GridToWorld(e->GetGridPos().x, e->GetGridPos().y);
		}
		fighters_.push_back(std::move(e));
	}
}

void PhaseCommon::Finalize()
{
	// Finalize を呼んでからコンテナをクリア
	for (auto& f : fighters_) {
		if (f) f->Finalize();
	}
	fighters_.clear();
}

void PhaseCommon::Update()
{
	// 各ファイターを更新
	for (auto& f : fighters_) {
		if (f) f->Update();
	}

	// 死亡したファイターを除去（Finalize を呼んでから erase）
	fighters_.erase(
		std::remove_if(fighters_.begin(), fighters_.end(),
			[](const std::unique_ptr<BaseFighter>& f) {
				if (!f) return true;
				Status* s = f->GetStatus();
				if (!s || !s->IsAlive()) {
					f->Finalize();
					return true;
				}
				return false;
			}),
		fighters_.end()
	);
}

void PhaseCommon::Draw()
{
	for (auto& f : fighters_) {
		if (f) f->Draw();
	}
}

Knight* PhaseCommon::GetKnight()
{
	for (auto& f : fighters_) {
		if (!f) continue;
		Status* s = f->GetStatus();
		if (s && s->faction == Faction::Player) {
			return dynamic_cast<Knight*>(f.get());
		}
	}
	return nullptr;
}

Knight* PhaseCommon::GetEnemyKnight()
{
	for (auto& f : fighters_) {
		if (!f) continue;
		Status* s = f->GetStatus();
		if (s && s->faction == Faction::Enemy) {
			return dynamic_cast<Knight*>(f.get());
		}
	}
	return nullptr;
}
