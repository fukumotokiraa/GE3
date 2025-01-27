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
		 
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
		 
#include "externals/DirectXTex/d3dx12.h"
		 
#include <cstdint>
#include <vector>

#include "StringUtility.h"

#pragma comment(lib,"dxcompiler.lib")



Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	return{ scale.x,0,0,0,
		   0,scale.y,0,0,
		   0,0,scale.z,0,
		   0,0,0,1 };
};

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	return { 1,0,0,0,
			0,1,0,0,
			0,0,0,0,
			translate.x,translate.y,translate.z,1 };
};

float LengthSquared(const Vector3& v) {
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	OutputDebugStringA("Hello,DirectX!\n");

	WinApp* winApp = nullptr;

	Input* input = nullptr;

	DirectXCommon* dxCommon = nullptr;

	TextureManager* texture = nullptr;

	Object3dCommon* object3dCommon = nullptr;

	//ModelCommon* modelCommon = nullptr;

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
	////Sprite* sprite = new Sprite();
	//sprite->Initialize(spriteCommon, "resources/monsterBall.png");
	//sprites.push_back(sprite);
	//sprites[6]->SetSize({ 100.0f,100.0f });
	//sprites[6]->SetPosition({ 6 * 150.0f,0.0f });

	Model* model = new Model();
	//model->Initialize(ModelManager::GetInstance()->GetModelCommon(), "axis", "axis.obj");

	Object3d* object3d = new Object3d();
	object3d->Initialize(object3dCommon,model);


	ModelManager::GetInstance()->LoadModel("axis.obj");
	object3d->SetModel("axis.obj");


#pragma endregion

	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};


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
		ImGui::Begin("Triangle Color Picker");           // ウィンドウの開始
		//ImGui::Checkbox("MonsterBall", &isChecked);
		//ImGui::Checkbox("Light", &materialData->enableLighting);
		//ImGui::DragFloat3("LightDirection", &directionalLightData->direction.x, 0.01f);
		//directionalLightData->direction = Normalize(directionalLightData->direction);
		ImGui::DragFloat3("ModelPosition", &object3d->GetTransform().translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("ModelRotate", &object3d->GetTransform().rotate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("ModelScale", &object3d->GetTransform().scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
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

		//Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
		//uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
		//uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
		//materialDataSprite->uvTransform = uvTransformMatrix;
		//描画前処理
		//DirectXの描画準備。全ての描画に共通のグラフィックスコマンドを積む
		dxCommon->PreDraw();

		//3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
		object3dCommon->DrawCommonSetting();

		//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
		spriteCommon->DrawCommonSetting();

		sprite->Draw();
		for (uint32_t i = 0; i < 5; i++) {
			sprites[i]->Draw();
			//sprites.push_back(sprite);
		}
		object3d->Draw();

		//ImGuiの内部コマンドを生成する
		ImGui::Render();



		//wvp用のCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

		//commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU2);
		//commandList->IASetIndexBuffer(&indexBufferViewSphere); // IBVを設定

		//commandList->DrawIndexedInstanced(kNumVertex, 1, 0, 0, 0);
		//描画（DrawCall/ドローコール）。３頂点で１つのインスタンス。インスタンスについては今後
		//commandList->DrawInstanced(6, 1, 0, 0);


		////マテリアルCBufferの場所を設定
		//commandList->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());






		////描画
		//commandList->DrawInstanced(6, 1, 0, 0);


		//実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandlist());

		sprite->Update();
		for (uint32_t i = 0; i < 5; i++) {
			sprites[i]->Update();
			//sprites.push_back(sprite);
		}

		object3d->Update();


		dxCommon->PostDraw();
	}

	//ImGuiの終了処理。
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	//signatureBlob->Release();
	//if (errorBlob)
	//{
	//	errorBlob->Release();
	//}
	//vertexShaderBlob->Release();
	//pixelShaderBlob->Release();


	ModelManager::GetInstance()->Finalize();
	winApp->Finalize();

	delete object3d;
	delete model;
	TextureManager::GetInstance()->Finalize();
	for (uint32_t i = 0; i < 5; i++) {
		delete sprites[i];
	}
	delete sprite;
	delete spriteCommon;
	delete input;
	delete winApp;
	CloseHandle(dxCommon->GetFenceEvent());
	delete dxCommon;
	delete object3dCommon;

	D3DResourceLeakChecker leakCheck;

	return 0;
}
