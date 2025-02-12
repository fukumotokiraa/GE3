#include "GameManeger.h"

/// コンストラクタによる初期化処理
GameManeger::GameManeger() {

	//各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();//シーンタイトル
	sceneArr_[STAGE] = std::make_unique<StageScene>();//シーンゲーム
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();//シーンクリア

	//初期シーンをタイトルに設定する
	currentSceneNo_ = TITLE;
	prevSceneNo_ = 0;
}

//デストラクタによるdelete処理
GameManeger::~GameManeger(){}

void GameManeger::changeScene(bool keys)
{
	//シーンのチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

	//シーン変更チェック
	if (prevSceneNo_ != currentSceneNo_) {
		sceneArr_[currentSceneNo_]->Initialize();
	}

	//シーンごとの更新処理
	sceneArr_[currentSceneNo_]->Update(keys);

	//シーンごとの描画処理
	sceneArr_[currentSceneNo_]->Draw();
}


