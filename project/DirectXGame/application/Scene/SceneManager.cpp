#include "SceneManager.h"
#include "GameScene.h"
#include "Object3dCommon.h"

SceneManager::~SceneManager()
{
	if (scene_) {
		scene_->Finalize();
		delete scene_;
	}
}

void SceneManager::Update()
{
	if (nextScene_) {
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		scene_ = nextScene_;
		nextScene_ = nullptr;

		scene_->SetSceneManager(this);
		scene_->SetSpriteCommon(spriteCommon_);
		scene_->SetObject3dCommon(object3dCommon_);
		scene_->SetInput(input_);
		scene_->Initialize();

		// GameScene に切り替わった瞬間のみカメラ Y を固定する
		if (auto gameScene = dynamic_cast<GameScene*>(scene_)) {
			if (object3dCommon_) {
				Camera* cam = object3dCommon_->GetDefaultCamera();
				if (cam) {
					Vector3 pos = cam->GetTranslate();
					pos.y = 86.0f;
					cam->SetTranslate(pos);
					cam->Update(); // 行列を即時更新
				}
			}
		}
	}
	if (scene_) scene_->Update();
}

void SceneManager::Draw()
{
	if (scene_) scene_->Draw();
}
