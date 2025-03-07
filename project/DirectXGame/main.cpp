#define _USE_MATH_DEFINES
#include <math.h>
#include  "Calculation.h"
#include "Logger.h"
#include "D3DResourceLeakChecker.h"
#include "Object3d.h"
#include "ModelCommon.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "MyGame.h"
		 
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
		 
#include "externals/DirectXTex/d3dx12.h"
		 
#include <cstdint>
#include <vector>
#include <random>
#include <unordered_map>

#include "StringUtility.h"

#pragma comment(lib,"dxcompiler.lib")


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakCheck;

	OutputDebugStringA("Hello,DirectX!\n");

	MyGame game;

	bool isSprite = false;

	bool isModel = false;


	game.Initialize();


	///=====================================================/// 
	///		メインループ
	///=====================================================///
	while (true) {
		//windowsのメッセージ処理
		if (winApp->ProcessMessage())
		{
			//ゲームループを抜ける
			break;
		}

#pragma region Update
		input->Update();
		if (input->PushKey(DIK_RIGHT))
		{
			camera->GetTranslate().x += 0.01f;
		}
		if (input->PushKey(DIK_LEFT))
		{
			camera->GetTranslate().x -= 0.01f;
		}
		if (input->PushKey(DIK_UP))
		{
			camera->GetTranslate().y += 0.01f;
		}
		if (input->PushKey(DIK_DOWN))
		{
			camera->GetTranslate().y -= 0.01f;
		}

		sprite->Update();
		for (uint32_t i = 0; i < 5; i++) {
			sprites[i]->Update();
		}

		object3d->Update();
		object3d2->Update();

		camera->Update();

		particleEmitter.Update();
		ParticleManager::GetInstance()->Update();

#pragma endregion

#pragma region ImGuiUpdate
		imguiManager->Begin();

#ifdef USE_IMGUI
		ImGui::ShowDemoWindow();
		ImGui::Begin("Camera");
		//ImGui::Checkbox("MonsterBall", &isChecked);
		//ImGui::Checkbox("Light", &materialData->enableLighting);
		//ImGui::DragFloat3("LightDirection", &directionalLightData->direction.x, 0.01f);
		//directionalLightData->direction = Normalize(directionalLightData->direction);
		Vector3 cameraPosition = camera->GetTranslate();
		Vector3 cameraRotate = camera->GetRotate();
		Vector3 cameraScale = camera->GetScale();
		ImGui::DragFloat3("CameraPosition", &cameraPosition.x, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("CameraScale", &cameraScale.x, 0.01f, -10.0f, 10.0f);
		camera->SetTranslate(cameraPosition);
		camera->SetRotate(cameraRotate);
		camera->SetScale(cameraScale);
		ImGui::End();

		ImGui::Begin("Model");
		ImGui::Checkbox("Draw", &isModel);
		ImGui::DragFloat3("ModelPosition", &object3d->GetTransform().translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("ModelRotate", &object3d->GetTransform().rotate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("ModelScale", &object3d->GetTransform().scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("Sprite");
		//ImGui::SetWindowSize(ImVec2(500, 100));
		ImGui::Checkbox("Draw", &isSprite);
		Vector2 position = sprite->GetPosition();
		ImGui::DragFloat2("SpritePosition", &position.x, 1.0f, -100.0f, 1000.0f, "%.1f");
		sprite->SetPosition(position);
		float rotation = sprite->GetRotation();
		ImGui::DragFloat("SpriteRotate", &rotation, 0.01f, 10.0f, 10.0f);
		sprite->SetRotation(rotation);
		Vector2 size = sprite->GetSize();
		ImGui::DragFloat2("SpriteSize", &size.x, 1.0f, 0.0f, 1000.0f);
		sprite->SetSize(size);
		Vector4 color = sprite->GetColor();
		ImGui::ColorEdit4("SpriteColor", &color.x);
		sprite->SetColor(color);
		ImGui::End();

		ImGui::Begin("Particle");
		bool useBillBoard = ParticleManager::GetInstance()->GetUseBillBoard();
		ImGui::Checkbox("BillBoard", &useBillBoard);
		ParticleManager::GetInstance()->SetUseBillBoard(useBillBoard);
		bool applyField = ParticleManager::GetInstance()->GetApplyField();
		ImGui::Checkbox("ApplyField", &applyField);
		ParticleManager::GetInstance()->SetApplyField(applyField);
		ImGui::End();
#endif

		imguiManager->End();
#pragma endregion

#pragma region PreDraw
		//描画前処理
		//DirectXの描画準備。全ての描画に共通のグラフィックスコマンドを積む
		dxCommon->PreDraw();

		srvManager->PreDraw();

		//3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
		object3dCommon->DrawCommonSetting();

		//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
		spriteCommon->DrawCommonSetting();

#pragma endregion

#pragma region Draw

		if (isSprite) {
			sprite->Draw();
		}
		//for (uint32_t i = 0; i < 5; i++) {
		//	sprites[i]->Draw();
		//}
		if (isModel) {
			object3d->Draw();
		}
		//object3d2->Draw();

		ParticleManager::GetInstance()->Draw();

#pragma endregion

#pragma region ImGuiDraw
		imguiManager->Draw();

#pragma endregion

#pragma region PostDraw
		dxCommon->PostDraw();

#pragma endregion

	}


	ParticleManager::GetInstance()->Finalize();
	imguiManager->Finalize();
	ModelManager::GetInstance()->Finalize();
	winApp->Finalize();

	delete object3d2;
	delete model2;
	delete object3d;
	delete model;
	TextureManager::GetInstance()->Finalize();
	for (uint32_t i = 0; i < 5; i++) {
		delete sprites[i];
	}
	delete sprite;
	delete spriteCommon;
	delete imguiManager;
	delete srvManager;
	delete camera;
	delete object3dCommon;
	delete dxCommon;
	delete input;
	delete winApp;



	return 0;
}
