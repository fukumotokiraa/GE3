#pragma once
#include <memory>
#include "IScene.h"
#include "TitleScene.h"
#include "StageScene.h"
#include "ClearScene.h"

/// GameManegerクラス
class GameManeger
{
public:
	GameManeger(); //コンストラクタ
	~GameManeger(); //デストラクタ

	void changeScene(bool keys);

	int GetCurrentSceneNo() {
		return currentSceneNo_;
	}

private:
	//シーン保持メンバ関数
	std::unique_ptr<IScene> sceneArr_[3];

	//現在のシーン
	int currentSceneNo_; 

	//1つ前のシーン
	int prevSceneNo_; 

};

