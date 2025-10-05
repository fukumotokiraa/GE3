#pragma once
#include "Framework.h"
#include "Sprite.h"
#include "Model.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "Stage.h"
#include "Scene/GameScene.h"
#include "Scene/TitleScene.h"
#include "Scene/SceneManager.h"


class MyGame : public Framework
{
public:
	MyGame();
	~MyGame();

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	SceneManager* sceneManager_ = nullptr;
	Sprite* sprite = nullptr;
	std::vector<Sprite*>sprites;
	Model* model = nullptr;
	Object3d* object3d = nullptr;
	Model* model2 = nullptr;
	Object3d* object3d2 = nullptr;

	Model* board = nullptr;
	Object3d* boardObject = nullptr;

	Stage stage;

	TitleScene* titleScene = nullptr;
	GameScene* gameScene = nullptr;

	bool isSprite = false;
	bool isModel = false;

	bool endRequest_ = false;

};

