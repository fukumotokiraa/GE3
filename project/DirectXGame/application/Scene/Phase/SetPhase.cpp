#include "SetPhase.h"
#include "PhaseCommon.h"
#include "Fighter/Mage.h"
#include <algorithm>

void SetPhase::Initialize(PhaseCommon* phaseCommon)
{
	phaseCommon_ = phaseCommon;

}

void SetPhase::Finalize()
{
}

void SetPhase::Update()
{
    Mage* playerMage = phaseCommon_->GetFirstOfType<Mage>(Team::Player);
    if (!playerMage) return;

    Object3d* obj = playerMage->GetObject3d();
    if (!obj) return;

    // ステージ幅/高さに合わせて clamp（BattlePhase の定数に合わせる）
    const int kStageWidth = 8;
    const int kStageHeight = 8;

    int gx = static_cast<int>(obj->GetTransform().translate.x + 4.0f + 0.5f);
    int gy = static_cast<int>(obj->GetTransform().translate.z + 4.0f + 0.5f);

    gx = std::clamp(gx, 0, kStageWidth - 1);
    gy = std::clamp(gy, 0, kStageHeight - 1);

    playerMage->SetGridPos(gx, gy);
}

void SetPhase::Draw()
{
}
