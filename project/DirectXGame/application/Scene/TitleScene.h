#pragma once
#include "BaseScene.h"
#include "SceneManager.h"
#include "Input.h"

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
};

