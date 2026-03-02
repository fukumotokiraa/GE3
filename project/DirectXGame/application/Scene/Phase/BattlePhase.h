#pragma once

#include <vector>

class PhaseCommon;
class BaseFighter;
struct StagePos;

class BattlePhase
{
public:
	void Initialize(PhaseCommon* phaseCommon);
	void Finalize();
	void Update();
	void Draw();

private:
	PhaseCommon* phaseCommon_ = nullptr;

	// 移動タイマー周り
	float movementTimer_ = 0.0f;
	const float moveInterval_ = 0.5f; // 0.5秒に1マス

	// 補間用エントリ（表示のみを補間する）
	struct MovementEntry {
		BaseFighter* fighter = nullptr;
		StagePos startGrid{};
		StagePos endGrid{};
		float elapsed = 0.0f;
		float duration = 0.5f;
	};

	std::vector<MovementEntry> movingUnits_;
};

