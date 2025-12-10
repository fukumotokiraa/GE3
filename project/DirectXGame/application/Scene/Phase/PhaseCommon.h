#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Model.h"
#include "ModelManager.h"
#include "Sprite.h"
#include "Input.h"
#include "Object3dCommon.h"
#include "Fighter/BaseFighter.h"
#include "Fighter/Knight.h"

class PhaseCommon
{
public:
	void Initialize(Object3dCommon* object3dCommon, Input* input);
	void Finalize();
	void Update();
	void Draw();

	Input* GetInput() { return input_; }

	// 互換性のため：最初の Player / Enemy を返す（存在しなければ nullptr）
	Knight* GetKnight();
	Knight* GetEnemyKnight();

	// 全ファイターへの参照が必要ならこちらを使う
	const std::vector<std::unique_ptr<BaseFighter>>& GetFighters() const { return fighters_; }

private:
	Object3dCommon* object3dCommon_ = nullptr;
	std::vector<std::unique_ptr<BaseFighter>> fighters_;
	Input* input_ = nullptr;
};

