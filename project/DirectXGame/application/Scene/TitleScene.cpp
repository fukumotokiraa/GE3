#include "TitleScene.h"
#include "Framework.h"
#include "GameScene.h"

TitleScene::TitleScene() :particleEmitter(particleGroups, emissionInterval)
{
}

void TitleScene::Initialize()
{
	titleSprite = new Sprite();
	TextureManager::GetInstance()->LoadTexture("resources/Title.png");
	titleSprite->Initialize(spriteCommon_, "resources/Title.png");
	titleSprite->SetPosition({ 0.0f,0.0f, 100.0f});

	ModelManager::GetInstance()->LoadModel("Match.gltf");
	rightMatch = new Model();
	rightMatchObject = new Object3d();
	rightMatchObject->Initialize(object3dCommon_, rightMatch);
	rightMatchObject->SetModel("Match.gltf");
	rightMatchObject->GetTransform().scale = { 5.0f,5.0f,5.0f };
	rightMatchObject->GetTransform().translate = { 2.0f,0.0f,-1.0f };

	leftMatch = new Model();
	leftMatchObject = new Object3d();
	leftMatchObject->Initialize(object3dCommon_, leftMatch);
	leftMatchObject->SetModel("Match.gltf");
	leftMatchObject->GetTransform().scale = { 5.0f,5.0f,5.0f };
	leftMatchObject->GetTransform().translate = { -2.0f,0.0f,-1.0f };

	ParticleManager::GetInstance()->CreateParticleGroup("right", "resources/circle.png", "plane.obj");
	rightParticle.transform.translate = { -8.0f, 7.0f, 0.0f };
	rightParticleGroup.particles.push_back(rightParticle);
	particleGroups["right"] = rightParticleGroup;

	ParticleManager::GetInstance()->CreateParticleGroup("left", "resources/circle.png", "plane.obj");
	leftParticle.transform.translate = { 8.0f, 7.0f, 0.0f };
	leftParticleGroup.particles.push_back(leftParticle);
	particleGroups["left"] = leftParticleGroup;

}

void TitleScene::Finalize()
{
	//ParticleManager::GetInstance()->Finalize();
	delete leftMatchObject;
	delete leftMatch;
	delete rightMatchObject;
	delete rightMatch;
	delete titleSprite;
}

void TitleScene::Update()
{
	titleSprite->Update();
	rightMatchObject->Update();
	leftMatchObject->Update();

	particleEmitter.Update();

	//if (input_->TriggerKey(DIK_SPACE)) {
	//	BaseScene* scene = new GameScene();
	//	scene->SetObject3dCommon(object3dCommon_);
	//	sceneManager_->SetNextScene(scene);
	//}
}

void TitleScene::Draw()
{
	titleSprite->Draw();
	rightMatchObject->Draw();
	leftMatchObject->Draw();

	ParticleManager::GetInstance()->Draw();
}
