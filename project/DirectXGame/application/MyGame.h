#pragma once
#include "Framework.h"
#include "Sprite.h"
#include "Model.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "scene/GameScene.h"
#include "scene/TitleScene.h"


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
	enum Scene {
		Title,
		Game
	};
	Scene scene = Title;

	Sprite* backSprite1 = nullptr;
	Sprite* backSprite2 = nullptr;
	Sprite* RedHpSprite = nullptr;
	Sprite* GreenHpSprite = nullptr;
	Sprite* blackBack = nullptr;
	Sprite* title = nullptr;
	Sprite* pushSpace = nullptr;

	std::vector<Sprite*>sprites;
	Model* model = nullptr;
	Object3d* object3d = nullptr;
	Model* model2 = nullptr;
	Object3d* object3d2 = nullptr;
	std::unordered_map<std::string, ParticleGroup> particleGroups;
	ParticleGroup particleGroup;
	Particle particle;
	float emissionInterval = 1.0f;
	ParticleEmitter particleEmitter;

	bool isSprite = false;
	bool isModel = false;

	bool endRequest_ = false;

	float backSpritePos1 = 0;
	float backSpritePos2 = 0;

	float GreenHpSize = 0;
	float defaultGreenHpSize = 0;

	Vector2 numberSize = { 32.0f,64.0f };

	int number = 000000;

	float titlePos = -400.0f;

	int pushDraw = 0;

};

