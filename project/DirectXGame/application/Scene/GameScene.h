#pragma once
#include "BaseScene.h"
#include "PreGameScene.h"
#include "Stage.h"

class PreGameScene;
class Stage;
class GameScene : public BaseScene
{
public:
	~GameScene()override = default;
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

	PreGameScene* GetPreGameScene() { return preGameScene_; }
	Stage* GetStage() { return stage_; }

private:
	Stage* stage_ = nullptr;
	PreGameScene* preGameScene_ = nullptr;
};

