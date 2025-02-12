#include "ClearScene.h"

void ClearScene::Initialize() {}

void ClearScene::Update(bool keys) {
	//スペースキーを押した瞬間
	if (keys) {
		//シーンをTITLEに切り替える
		sceneNo = TITLE;
	}
}

void ClearScene::Draw() {
	//Novice::ScreenPrintf(0, 0, "Clear Scene");
	//Novice::ScreenPrintf(0, 30, "Push Space");
}
