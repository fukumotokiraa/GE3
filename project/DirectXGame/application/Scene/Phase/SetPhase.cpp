#include "SetPhase.h"
#include "PhaseCommon.h"

void SetPhase::Initialize(PhaseCommon* phaseCommon)
{
	phaseCommon_ = phaseCommon;

}

void SetPhase::Finalize()
{
}

void SetPhase::Update()
{
    if (phaseCommon_->GetInput()->TriggerKey(DIK_D))
    {
		phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.x += 1.0f;
    }
    if (phaseCommon_->GetInput()->TriggerKey(DIK_A))
    {
		phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.x -= 1.0f;
    }
    if (phaseCommon_->GetInput()->TriggerKey(DIK_W))
    {
		phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.z += 1.0f;
    }
    if (phaseCommon_->GetInput()->TriggerKey(DIK_S))
    {
		phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.z -= 1.0f;
    }
    phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.x = std::clamp(phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.x, -4.0f, 3.0f);
    phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.z = std::clamp(phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.z, -4.0f, 3.0f);
    phaseCommon_->GetKnight()->SetGridPos(
        static_cast<int>(phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.x + 4.0f + 0.5f),
        static_cast<int>(phaseCommon_->GetKnight()->GetKnightObject()->GetTransform().translate.z + 4.0f + 0.5f)
	);
}

void SetPhase::Draw()
{
}
