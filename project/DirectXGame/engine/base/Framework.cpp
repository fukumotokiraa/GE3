#include "Framework.h"

Framework::Framework()
{
}

void Framework::Initialize()
{
#pragma region 基盤システムの初期化

	winApp = new WinApp();
	winApp->Initialize();

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	input = new Input();
	input->Initialize(winApp);

	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);

	imguiManager = new ImGuiManager();
	imguiManager->Initialize(winApp, dxCommon, srvManager);

	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	camera = new Camera();
	camera->SetRotate({ 0.0f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	object3dCommon->SetDefaultCamera(camera);

	ModelManager::GetInstance()->Initialize(dxCommon);

	ParticleManager::GetInstance()->Initialize(dxCommon, srvManager, object3dCommon);

#pragma endregion
}

void Framework::Finalize()
{
	delete spriteCommon;
	delete imguiManager;
	delete srvManager;
	delete camera;
	delete object3dCommon;
	delete dxCommon;
	delete input;
	delete winApp;
}

void Framework::Update()
{
#pragma region ゲーム終了処理

	//windowsのメッセージ処理
	if (winApp->ProcessMessage())
	{
		//ゲームループを抜ける
		endRequest_ = true;
	}

#pragma endregion

	camera->Update();
	ParticleManager::GetInstance()->Update();
}

void Framework::Run()
{
	D3DResourceLeakChecker leakCheck;

	Initialize();

	while (true) {
		Update();
		if (IsEndRequest()) {
			break;
		}
		Draw();
	}

	Finalize();
}
