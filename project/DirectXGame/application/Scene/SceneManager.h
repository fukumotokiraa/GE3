#pragma once
#include "Scene/BaseScene.h"

class SceneManager
{
public:
	~SceneManager();
	void Update();
	void Draw();

	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	void SetSpriteCommon(SpriteCommon* spriteCommon) { spriteCommon_ = spriteCommon; }
	void SetObject3dCommon(Object3dCommon* object3dCommon) { object3dCommon_ = object3dCommon; }
	void SetInput(Input* input) { input_ = input; }

private:
	//現在のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	Object3dCommon* object3dCommon_ = nullptr;
	Input* input_ = nullptr;
};

