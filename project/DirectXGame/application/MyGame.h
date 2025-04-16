#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SrvManager.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Object3dCommon.h"
#include "Camera.h"
#include "Framework.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "Model.h"
#include "Object3d.h"
#include "ParticleEmitter.h"


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
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	SrvManager* srvManager = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	ImGuiManager* imguiManager = nullptr;
	Object3dCommon* object3dCommon = nullptr;
	Camera* camera = nullptr;

	Sprite* sprite = nullptr;
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

};

