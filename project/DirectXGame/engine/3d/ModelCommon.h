#pragma once
#include "DirectXCommon.h"

class ModelCommon
{
public:
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDXCommon() { return dxCommon_; }

private:
	DirectXCommon* dxCommon_;

};

