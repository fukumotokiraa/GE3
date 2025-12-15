#pragma once
#include "Fighter/Knight.h"
#include <algorithm>

class PhaseCommon;
class SetPhase
{
public:
	void Initialize(PhaseCommon* phaseCommon);
	void Finalize();
	void Update();
	void Draw();

private:
	PhaseCommon* phaseCommon_ = nullptr;

};

