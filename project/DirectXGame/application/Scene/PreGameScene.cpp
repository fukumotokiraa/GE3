#include "PreGameScene.h"

void PreGameScene::Initialize(SpriteCommon* spriteCommon)
{
	setSprite = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/set.png");
	setSprite->Initialize(spriteCommon, "resources/set.png");
	setSprite->SetPosition({ 0.0f,0.0f, 0.0f });
	setSprite->SetScale({ 0.0f, 0.0f });
	setSpriteScale_ = 0.0f;
	isSetSpriteAppearing_ = false;
	setSpriteAppearTimer_ = 0.0f;
}

void PreGameScene::Finalize()
{
	delete setSprite;
}

void PreGameScene::Update()
{
    setSprite->Update();

    // 拡大イージング
    if (isSetSpriteAppearing_) {
        setSpriteAppearTimer_ += 1.0f / 60.0f;
        float t = std::clamp(setSpriteAppearTimer_ / setSpriteAppearDuration_, 0.0f, 1.0f);
        float s = 1.70158f;
        t -= 1.0f;
        setSpriteScale_ = t * t * ((s + 1) * t + s) + 1.0f;
        setSprite->SetScale({ setSpriteScale_, setSpriteScale_ });
        if (setSpriteAppearTimer_ >= setSpriteAppearDuration_) {
            setSpriteScale_ = 1.0f;
            setSprite->SetScale({ 1.0f, 1.0f });
            isSetSpriteAppearing_ = false;
            setSpriteStayTimer_ = 0.0f;
        }
        return;
    }

    // 表示中
    if (setSpriteScale_ >= 1.0f && !isSetSpriteDisappearing_) {
        setSpriteStayTimer_ += 1.0f / 60.0f;
        if (setSpriteStayTimer_ >= setSpriteStayDuration_) {
            isSetSpriteDisappearing_ = true;
            setSpriteDisappearTimer_ = 0.0f;
        }
        return;
    }

    // 縮小イージング
    if (isSetSpriteDisappearing_) {
        setSpriteDisappearTimer_ += 1.0f / 60.0f;
        float t = std::clamp(setSpriteDisappearTimer_ / setSpriteDisappearDuration_, 0.0f, 1.0f);
        float s = 1.70158f;
        // BackEaseIn
        setSpriteScale_ = 1.0f - (t * t * ((s + 1) * t - s));
        setSpriteScale_ = std::clamp(setSpriteScale_, 0.0f, 1.0f);
        setSprite->SetScale({ setSpriteScale_, setSpriteScale_ });
        if (setSpriteDisappearTimer_ >= setSpriteDisappearDuration_) {
            setSpriteScale_ = 0.0f;
            setSprite->SetScale({ 0.0f, 0.0f });
            isSetSpriteDisappearing_ = false;
        }
        return;
    }
}

void PreGameScene::Draw()
{
	//setSprite->Draw();
}

void PreGameScene::StartSetSpriteAppear()
{
    isSetSpriteAppearing_ = true;
    isSetSpriteDisappearing_ = false;
    setSpriteAppearTimer_ = 0.0f;
    setSpriteDisappearTimer_ = 0.0f;
    setSpriteStayTimer_ = 0.0f;
    setSpriteScale_ = 0.0f;
    setSprite->SetScale({ 0.0f, 0.0f });
}
