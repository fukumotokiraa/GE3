#include "TitleScene.h"

void TitleScene::Initialize() {}

void TitleScene::Update(bool keys) {

	//スペースキーを押した瞬間
	if (keys) {
		//シーンをSTAGEに切り替える
		IScene::sceneNo = STAGE;
	}
}

void TitleScene::Draw() {
	//Novice::ScreenPrintf(0, 0, "Title Scene");
	//Novice::ScreenPrintf(0, 30, "Push Space");
}

