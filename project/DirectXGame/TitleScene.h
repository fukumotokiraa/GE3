#pragma once
#include "IScene.h"

/// <summary>
/// ISceneクラスを継承したTitleSceneを宣言
/// </summary>
class TitleScene : public IScene
{
public://メンバ関数

	///初期化処理
	void Initialize() override;

	///更新処理
	void Update(bool keys) override;

	///描画処理
	void Draw() override;

};

