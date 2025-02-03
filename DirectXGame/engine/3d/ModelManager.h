#pragma once
#include <map>
#include <string>

#include "Model.h"

class ModelManager
{
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	//シングルトンインスタンスの取得
	static ModelManager* GetInstance();
	//終了
	void Finalize();
	//モデルファイルの読み込み
	void LoadModel(const std::string& filePath);

	Model* FindModel(const std::string& filePath);

	ModelCommon* GetModelCommon() { return modelCommon_; }

private:
	ModelCommon* modelCommon_ = nullptr;

	static ModelManager* instance_;
	//コンストラクタ
	ModelManager() = default;
	//デストラクタ
	~ModelManager() = default;
	//コピーコンストラクタ
	ModelManager(const ModelManager& rhs) = delete;
	//コピー代入演算子
	ModelManager& operator=(const ModelManager& rhs) = delete;
	//モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models_;

};

