#include "PhaseCommon.h"

void PhaseCommon::Initialize(Object3dCommon* object3dCommon, Input* input)
{
	object3dCommon_ = object3dCommon;
	input_ = input;

	knight_ = new Knight();
	knight_->SetObject3dCommon(object3dCommon_);
	knight_->Initialize();

}

void PhaseCommon::Finalize()
{
	delete knight_;
}

void PhaseCommon::Update()
{
	knight_->Update();
}

void PhaseCommon::Draw()
{
	knight_->Draw();
}
