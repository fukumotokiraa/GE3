#pragma once

class PhaseCommon;
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
};

