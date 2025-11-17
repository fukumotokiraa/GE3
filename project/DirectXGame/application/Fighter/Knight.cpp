#include "Knight.h"

void Knight::Initialize()
{
	ModelManager::GetInstance()->LoadModel("cube.obj");
	knightModel = new Model();
	knightObject = new Object3d();
	knightObject->Initialize(object3dCommon_, knightModel);
	knightObject->SetModel("cube.obj");
	knightObject->GetTransform().translate = { 0.0f,1.0f,0.0f };

}

void Knight::Finalize()
{
	delete knightObject;
	delete knightModel;
}

void Knight::Update()
{
	knightObject->Update();
}

void Knight::Draw()
{
	knightObject->Draw();
}
