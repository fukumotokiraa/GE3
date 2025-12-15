#include "PhaseCommon.h"
#include <stdexcept>
#include "Fighter/Knight.h"
#include "Fighter/Mage.h"
#include "Fighter/FighterFactory.h"

void PhaseCommon::Initialize(Object3dCommon* object3dCommon, Input* input)
{
	object3dCommon_ = object3dCommon;
	input_ = input;
	// 必要なら既存の fighters_ 初期化処理を追加
}

void PhaseCommon::Finalize()
{
	// すべてのファイターを破棄してマップをクリア
	fighters_.clear();
	typeMap_.clear();
	teamMap_.clear();
	object3dCommon_ = nullptr;
	input_ = nullptr;
}

void PhaseCommon::Update()
{
	// まず全ファイターを更新
	for (auto& f : fighters_) {
		if (f) { f->Update(); }
	}

	// その後、死亡したファイターを片付ける（Finalize を呼んで map からも消す）
	for (auto it = fighters_.begin(); it != fighters_.end(); ) {
		BaseFighter* p = it->get();
		if (p) {
			Status* s = p->GetStatus();
			if (s && !s->IsAlive()) {
				// 後片付け
				p->Finalize();
				typeMap_.erase(p);
				teamMap_.erase(p);
				it = fighters_.erase(it);
				continue;
			}
		}
		++it;
	}
}

void PhaseCommon::Draw()
{
	for (auto& f : fighters_) {
		if (f) { f->Draw(); }
	}
}

BaseFighter* PhaseCommon::SpawnFighter(FighterType type, Team team)
{
	// Factory で生成
	auto fighter = FighterFactory::Create(type);
	if (!fighter) {
		throw std::runtime_error("Failed to create fighter");
	}

	// 所有権を保持するために vector に push
	fighters_.push_back(std::move(fighter));
	BaseFighter* ptr = fighters_.back().get();

	// メタデータ登録
	typeMap_[ptr] = type;
	teamMap_[ptr] = team;

	// 重要: Object3dCommon を渡してから初期化する（Mage::Initialize が依存）
	ptr->SetObject3dCommon(object3dCommon_);

	// 初期化（BaseFighter::Initialize は引数無しの想定）
	ptr->Initialize();

	return ptr;
}

std::vector<BaseFighter*> PhaseCommon::GetFightersByTeam(Team team)
{
	std::vector<BaseFighter*> out;
	out.reserve(fighters_.size());
	for (auto& up : fighters_) {
		BaseFighter* p = up.get();
		if (!p) continue;
		auto it = teamMap_.find(p);
		if (it != teamMap_.end() && it->second == team) {
			out.push_back(p);
		}
	}
	return out;
}

std::vector<BaseFighter*> PhaseCommon::GetFightersByType(FighterType type)
{
	std::vector<BaseFighter*> out;
	out.reserve(fighters_.size());
	for (auto& up : fighters_) {
		BaseFighter* p = up.get();
		if (!p) continue;
		auto it = typeMap_.find(p);
		if (it != typeMap_.end() && it->second == type) {
			out.push_back(p);
		}
	}
	return out;
}
