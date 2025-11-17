#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "Input.h"

class SceneManager;
class BaseScene
{
public:
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetSpriteCommon(SpriteCommon* spriteCommon) { spriteCommon_ = spriteCommon; }
	void SetObject3dCommon(Object3dCommon* object3dCommon) { object3dCommon_ = object3dCommon; }
	void SetInput(Input* input) { input_ = input; }
	Input* GetInput() { return input_; }

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:
	SpriteCommon* spriteCommon_ = nullptr;
	Object3dCommon* object3dCommon_ = nullptr;
	Input* input_ = nullptr;

	SceneManager* sceneManager_ = nullptr;
};

