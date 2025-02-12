#pragma once

// シーン名を列挙で定義
enum Scene {
	TITLE,//シーンタイトル
	STAGE,//シーンステージ
	CLEAR,//シーンクリア
};

class Input;
// シーン内での処理を行う基底クラス
class IScene 
{
protected:
	//シーン番号を管理する変数
	static int sceneNo;
	Input* input_;

public:
	//継承先で実装される関数
	//抽象クラスなので純粋仮想関数

	// 初期化処理
	virtual void Initialize() = 0;

	// 更新処理
	virtual void Update(bool keys) = 0;

	// 描画処理
	virtual void Draw() = 0;

	// 仮想デストラクタ
	virtual ~IScene();

	// シーン番号ゲッタ
	int GetSceneNo();
};
