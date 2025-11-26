#include "Clear.h"

void Clear::Initialize(SpriteCommon* spriteCommon)
{
	clearSprite = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/clear.png");
	clearSprite->Initialize(spriteCommon, "resources/clear.png");
	clearSprite->SetPosition(clearStartPos);
	clearSpriteMoveState = ClearSpriteMoveState::Idle;
	clearSpriteMoveTimer = 0.0f;
}

void Clear::Finalize()
{
	delete clearSprite;
}

void Clear::Update()
{
    if (isClear && clearSpriteMoveState == ClearSpriteMoveState::Idle) {
        clearSpriteMoveState = ClearSpriteMoveState::Entering;
        clearSpriteMoveTimer = 0.0f;
    }

    // ClearSpriteのイージング演出
    switch (clearSpriteMoveState) {
    case ClearSpriteMoveState::Idle:
        // 何もしない
        break;
    case ClearSpriteMoveState::Entering:
        clearSpriteMoveTimer += 1.0f / 60.0f;
        {
            float t = std::clamp(clearSpriteMoveTimer / clearEnterDuration, 0.0f, 1.0f);
            float easeT = easeOutCubic(t);
            Vector3 pos = {
                clearStartPos.x + (clearCenterPos.x - clearStartPos.x) * easeT,
                clearStartPos.y + (clearCenterPos.y - clearStartPos.y) * easeT,
                clearStartPos.z
            };
            clearSprite->SetPosition(pos);
            if (t >= 1.0f) {
                clearSpriteMoveState = ClearSpriteMoveState::Staying;
                clearSpriteMoveTimer = 0.0f;
            }
        }
        break;
    case ClearSpriteMoveState::Staying:
        clearSpriteMoveTimer += 1.0f / 60.0f;
        clearSprite->SetPosition(clearCenterPos);
        if (clearSpriteMoveTimer >= clearStayDuration) {
            clearSpriteMoveState = ClearSpriteMoveState::Exiting;
            clearSpriteMoveTimer = 0.0f;
        }
        break;
    case ClearSpriteMoveState::Exiting:
        clearSpriteMoveTimer += 1.0f / 60.0f;
        {
            float t = std::clamp(clearSpriteMoveTimer / clearExitDuration, 0.0f, 1.0f);
            float easeT = easeOutCubic(t);
            Vector3 pos = {
                clearCenterPos.x + (clearEndPos.x - clearCenterPos.x) * easeT,
                clearCenterPos.y + (clearEndPos.y - clearCenterPos.y) * easeT,
                clearCenterPos.z
            };
            clearSprite->SetPosition(pos);
            if (t >= 1.0f) {
                clearSpriteMoveState = ClearSpriteMoveState::Idle;
                clearSprite->SetPosition(clearStartPos);
                // クリア演出終了後の処理があればここに
            }
        }
        break;
    }
    clearSprite->Update();
}

void Clear::Draw()
{
    if (clearSpriteMoveState != ClearSpriteMoveState::Idle) {
        clearSprite->Draw();
    }
}
