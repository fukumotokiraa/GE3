#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include  "Calculation.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Logger.h"
#include "D3DResourceLeakChecker.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Object3dCommon.h"
#include "Object3d.h"
#include "ModelCommon.h"
#include "Model.h"
#include "ModelManager.h"
#include "GameManeger.h"
		 
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
		 
#include "externals/DirectXTex/d3dx12.h"
		 
#include <cstdint>
#include <vector>

#include "StringUtility.h"

#pragma comment(lib,"dxcompiler.lib")


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakCheck;

	OutputDebugStringA("Hello,DirectX!\n");

	WinApp* winApp = nullptr;

	Input* input = nullptr;

	DirectXCommon* dxCommon = nullptr;

	TextureManager* texture = nullptr;

	Object3dCommon* object3dCommon = nullptr;

	const uint32_t kSubdivision = 16;
	const uint32_t kNumVertex = kSubdivision * kSubdivision * 6;

#pragma region 基盤システムの初期化

	winApp = new WinApp();
	winApp->Initialize();

	input = new Input();
	input->Initialize(winApp);

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	TextureManager::GetInstance()->Initialize(dxCommon);

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	//3Dオブジェクト共通部の初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	ModelManager::GetInstance()->Initialize(dxCommon);

	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");
#pragma endregion

#pragma region 最初のシーンの初期化

	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon, "resources/uvChecker.png");

	std::vector<Sprite*>sprites;
	for (uint32_t i=0;i<5;i++) {
		Sprite* sprite = new Sprite();
		if (i % 2 == 0) {
			sprite->Initialize(spriteCommon, "resources/uvChecker.png");
		}
		else {
			sprite->Initialize(spriteCommon, "resources/monsterBall.png");
		}
		sprites.push_back(sprite);
		sprites[i]->SetSize({ 100.0f,100.0f });
		sprites[i]->SetPosition({ i * 150.0f,0.0f });
	}

	Model* model = new Model();

	Object3d* object3d = new Object3d();
	object3d->Initialize(object3dCommon,model);

	GameManeger* gameManeger = new GameManeger();


	ModelManager::GetInstance()->LoadModel("axis.obj");
	object3d->SetModel("axis.obj");


#pragma endregion

	while (true) {
		//windowsのメッセージ処理
		if (winApp->ProcessMessage())
		{
			//ゲームループを抜ける
			break;
		}
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//ゲームの処理
		// ImGui UIの追加
		ImGui::Begin("sceneNo");
		int sceneNo = gameManeger->GetCurrentSceneNo();
		ImGui::Text("scene: %d", sceneNo);
		ImGui::End();

		if (sceneNo == 1) {
		ImGui::Begin("Model");           // ウィンドウの開始
		ImGui::DragFloat3("ModelPosition", &object3d->GetTransform().translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("ModelRotate", &object3d->GetTransform().rotate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("ModelScale", &object3d->GetTransform().scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::End();                     // ウィンドウの終了

		ImGui::Begin("Sprite");           // ウィンドウの開始
		Vector2 position = sprite->GetPosition();
		ImGui::DragFloat2("SpritePosition", &position.x, 1.0f, -100.0f, 1000.0f);
		sprite->SetPosition(position);
		float rotation = sprite->GetRotation();
		ImGui::DragFloat("SpriteRotate", &rotation, 0.01f, 10.0f, 10.0f);
		sprite->SetRotation(rotation);
		Vector4 color = sprite->GetColor();
		ImGui::ColorEdit4("SpriteColor", &color.x);
		sprite->SetColor(color);
		Vector2 size = sprite->GetSize();
		ImGui::DragFloat2("SpriteSize", &size.x, 1.0f, 0.0f, 1000.0f);
		sprite->SetSize(size);
		ImGui::End();
		}

		input->Update();
		if (input->PushKey(DIK_RIGHT))
		{
			object3d->GetTransform().translate.x += 0.01f;
		}
		if (input->PushKey(DIK_LEFT))
		{
			object3d->GetTransform().translate.x -= 0.01f;
		}
		if (input->PushKey(DIK_UP))
		{
			object3d->GetTransform().translate.y += 0.01f;
		}
		if (input->PushKey(DIK_DOWN))
		{
			object3d->GetTransform().translate.y -= 0.01f;
		}

		//描画前処理
		//DirectXの描画準備。全ての描画に共通のグラフィックスコマンドを積む
		dxCommon->PreDraw();

		//3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
		object3dCommon->DrawCommonSetting();

		//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
		spriteCommon->DrawCommonSetting();

		if (sceneNo == 1) {
			sprite->Draw();
			for (uint32_t i = 0; i < 5; i++) {
				sprites[i]->Draw();
			}
			object3d->Draw();
		}

		//ImGuiの内部コマンドを生成する
		ImGui::Render();


		//実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandlist().Get());

		sprite->Update();
		for (uint32_t i = 0; i < 5; i++) {
			sprites[i]->Update();
		}

		object3d->Update();

		gameManeger->changeScene(input->TriggerKey(DIK_SPACE));

		dxCommon->PostDraw();
	}

	//ImGuiの終了処理。


	ModelManager::GetInstance()->Finalize();
	winApp->Finalize();

	delete gameManeger;
	delete object3d;
	delete model;
	TextureManager::GetInstance()->Finalize();
	for (uint32_t i = 0; i < 5; i++) {
		delete sprites[i];
	}
	delete sprite;
	delete spriteCommon;
	delete object3dCommon;
	delete dxCommon;
	delete input;
	delete winApp;



	return 0;
}
