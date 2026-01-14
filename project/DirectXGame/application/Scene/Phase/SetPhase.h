#pragma once
#include "Fighter/Knight.h"
#include <algorithm>

class PhaseCommon;
class Model;
class Object3d;
class BaseFighter;

class SetPhase
{
public:
	void Initialize(PhaseCommon* phaseCommon);
	void Finalize();
	void Update();
	void Draw();

private:
	PhaseCommon* phaseCommon_ = nullptr;

	// フレーム選択用モデル / オブジェクト
	Model* frameModel_ = nullptr;
	Model* dframeModel_ = nullptr;
	Object3d* frameObject_ = nullptr;

	// ステージ変換パラメータ（Stage と合わせる）
	const float kOriginX_ = -4.0f;
	const float kOriginY_ = -4.0f;
	const float kBlockSize_ = 1.0f;
	const float kFrameHeight_ = 0.5f; // フレームをわずかに浮かせて描画
	const float kUnitWorldHeight_ = 1.0f; // ユニットの高さ（ワールド座標 Y）

	// グリッドサイズ（Stage と合わせる）
	const int kStageWidth_ = 8;
	const int kStageHeight_ = 8;

	// カーソル（フレーム）のグリッド座標
	int cursorX_ = 0;
	int cursorY_ = 0;

	// 選択中のユニット（nullptr なら未選択）
	BaseFighter* selectedFighter_ = nullptr;
	bool isHolding_ = false;
};

