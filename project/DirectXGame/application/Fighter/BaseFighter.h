#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Status.h"

class BaseFighter
{
public:
	virtual ~BaseFighter() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual Status* GetStatus() = 0;

	void SetObject3dCommon(Object3dCommon* object3dCommon) { object3dCommon_ = object3dCommon; }

	void SetGridPos(int x, int y) { stagePos_.x = x; stagePos_.y = y; }
	StagePos GetGridPos() const { return stagePos_; }

	// 追加: 外部から faction を設定する簡易ヘルパ
	void SetFaction(Faction f) {
		Status* s = GetStatus();
		if (s) s->faction = f;
	}


protected:
	Object3dCommon* object3dCommon_ = nullptr;

private:
	StagePos stagePos_ = { 0,0 };

};

