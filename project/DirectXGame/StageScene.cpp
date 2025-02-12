#include "StageScene.h"

void StageScene::Initialize() {}

void StageScene::Update(bool keys) {

	//スペースキーを押した瞬間
	if (keys) {
		//シーンをCLEARに変える
		IScene::sceneNo = CLEAR;
	}
}

void StageScene::Draw() {
	//Novice::ScreenPrintf(0, 0, "Stage Scene");
	//Novice::ScreenPrintf(0, 30, "Push Space");
}







