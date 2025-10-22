#pragma once
#include "BaseScene.h"

class PreGameScene
{
public:
	void Initialize(SpriteCommon* spriteCommon);
	void Finalize();
	void Update();
	void Draw();

	void StartSetSpriteAppear();

private:
    Sprite* setSprite = nullptr;
    float setSpriteScale_ = 0.0f;
    bool isSetSpriteAppearing_ = false;
    bool isSetSpriteDisappearing_ = false;
    float setSpriteAppearTimer_ = 0.0f;
    float setSpriteDisappearTimer_ = 0.0f;
    const float setSpriteAppearDuration_ = 0.5f;
    const float setSpriteStayDuration_ = 1.0f;
    const float setSpriteDisappearDuration_ = 0.5f;
    float setSpriteStayTimer_ = 0.0f;

};

