#pragma once
#include "BaseScene.h"
#include "Stage.h"

class Stage;
class GameScene : public BaseScene
{
public:
	~GameScene()override = default;
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

	void SetObject3dCommon(Object3dCommon* object3dCommon) { object3dCommon_ = object3dCommon; }

private:
	Stage stage;
	Object3dCommon* object3dCommon_ = nullptr;
};

