#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include "Object3dCommon.h"
#include "Input.h"
#include "Fighter/FighterFactory.h"
#include "Fighter/BaseFighter.h"

// forward declarations for compatibility getters
class Knight;
class Mage;

enum class Team { Player, Enemy, Neutral };

class PhaseCommon
{
public:
	void Initialize(Object3dCommon* object3dCommon, Input* input);
	void Finalize();
	void Update();
	void Draw();

	Input* GetInput() { return input_; }

	// 追加: Object3dCommon の外部参照を取得可能にする（SetPhase で使用）
	Object3dCommon* GetObject3dCommon() { return object3dCommon_; }

	// 全ファイターへの参照
	const std::vector<std::unique_ptr<BaseFighter>>& GetFighters() const { return fighters_; }

	// 汎用スポーン / クエリ
	BaseFighter* SpawnFighter(FighterType type, Team team);
	std::vector<BaseFighter*> GetFightersByTeam(Team team);
	std::vector<BaseFighter*> GetFightersByType(FighterType type);

	// ---- ここから追加: テンプレートベースの汎用取得 ----
	// 指定型の最初のインスタンスを返す（team を指定しなければ最初に見つかったもの）
	template<typename T>
	T* GetFirstOfType(Team team = Team::Neutral)
	{
		for (auto& up : fighters_) {
			if (auto p = dynamic_cast<T*>(up.get())) {
				if (team == Team::Neutral) return p;
				auto it = teamMap_.find(p);
				if (it != teamMap_.end() && it->second == team) return p;
			}
		}
		return nullptr;
	}

	// 指定型の全インスタンスを返す（team を指定すると絞り込む）
	template<typename T>
	std::vector<T*> GetAllOfType(Team team = Team::Neutral)
	{
		std::vector<T*> out;
		out.reserve(fighters_.size());
		for (auto& up : fighters_) {
			if (auto p = dynamic_cast<T*>(up.get())) {
				if (team == Team::Neutral) { out.push_back(p); continue; }
				auto it = teamMap_.find(p);
				if (it != teamMap_.end() && it->second == team) out.push_back(p);
			}
		}
		return out;
	}
	// ---- ここまで追加 ----


private:
	Object3dCommon* object3dCommon_ = nullptr;
	std::vector<std::unique_ptr<BaseFighter>> fighters_;
	Input* input_ = nullptr;

	// メタ情報（fighters_ が所有）
	std::unordered_map<BaseFighter*, FighterType> typeMap_;
	std::unordered_map<BaseFighter*, Team> teamMap_;
};

