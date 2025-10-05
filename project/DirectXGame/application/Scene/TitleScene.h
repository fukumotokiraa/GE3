#pragma once
#include "BaseScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Model.h"
#include "Object3d.h"
#include "ParticleEmitter.h"

class GameScene;
class TitleScene : public BaseScene
{
public:
	~TitleScene()override = default;
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;



private:
	Sprite* titleSprite = nullptr;
	Model* rightMatch = nullptr;
	Object3d* rightMatchObject = nullptr;
	Model* leftMatch = nullptr;
	Object3d* leftMatchObject = nullptr;

};

