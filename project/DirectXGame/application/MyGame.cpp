#include "MyGame.h"

MyGame::MyGame()
	:particleEmitter(particleGroups, emissionInterval) {
}

MyGame::~MyGame()
{
}

void MyGame::Initialize()
{
	Framework::Initialize();

	titleScene->Initialize();
	gameScene->Initialize();

#pragma region 各オブジェクトの初期化

	//TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	//TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("resources/sky.png");
	TextureManager::GetInstance()->LoadTexture("resources/RedHP.png");
	TextureManager::GetInstance()->LoadTexture("resources/GreenHP.png");
	TextureManager::GetInstance()->LoadTexture("resources/number.png");

	backSprite1 = new Sprite();
	backSprite1->Initialize(spriteCommon, "resources/sky.png");
	backSprite1->SetPosition({ 0.0f,0.0f });
	backSpritePos1 = backSprite1->GetPosition().x;

	backSprite2 = new Sprite();
	backSprite2->Initialize(spriteCommon, "resources/sky.png");
	backSprite2->SetPosition({ 1280.0f,0.0f });
	backSpritePos2 = backSprite2->GetPosition().x;

	RedHpSprite = new Sprite();
	RedHpSprite->Initialize(spriteCommon, "resources/RedHP.png");
	RedHpSprite->SetPosition({ 50.0f, 50.0f });

	GreenHpSprite = new Sprite();
	GreenHpSprite->Initialize(spriteCommon, "resources/GreenHP.png");
	GreenHpSprite->SetPosition({ 50.0f, 50.0f });
	defaultGreenHpSize = GreenHpSprite->GetSize().x;

	for (uint32_t i = 0; i < 5; i++) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteCommon, "resources/number.png");
		sprite->SetSize(numberSize);
		sprite->SetTextureLeftTop({ 0.0f, 0.0f });
		sprite->SetTextureSize(numberSize);
		sprites.push_back(sprite);
		sprites[i]->SetPosition({ 700.0f + i * numberSize.x,0.0f });
	}

	model = new Model();
	object3d = new Object3d();
	object3d->Initialize(object3dCommon, model);
	ModelManager::GetInstance()->LoadModel("Player.gltf");
	object3d->SetModel("Player.gltf");

	//model2 = new Model();
	//object3d2 = new Object3d();
	//object3d2->Initialize(object3dCommon, model2);
	//ModelManager::GetInstance()->LoadModel("plane.obj");
	//object3d2->SetModel("plane.obj");

	//ParticleManager::GetInstance()->CreateParticleGroup("example", "resources/circle.png", "plane.obj");
	//particle.transform.translate = { 0.0f, 0.0f, 0.0f };
	//particleGroup.particles.push_back(particle);
	//particleGroups["example"] = particleGroup;

#pragma endregion

}

void MyGame::Finalize()
{
#pragma region Finalize

	ParticleManager::GetInstance()->Finalize();
	imguiManager->Finalize();
	ModelManager::GetInstance()->Finalize();
	winApp->Finalize();

	//delete object3d2;
	//delete model2;
	delete object3d;
	delete model;
	TextureManager::GetInstance()->Finalize();
	for (uint32_t i = 0; i < 5; i++) {
		delete sprites[i];
	}
	delete GreenHpSprite;
	delete RedHpSprite;
	delete backSprite2;
	delete backSprite1;

#pragma endregion

	gameScene->Finalize();
	titleScene->Finalize();

	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();
#pragma region Update
	input->Update();
	if (input->PushKey(DIK_D))
	{
		object3d->GetTransform().translate.x += 0.1f;
	}
	if (input->PushKey(DIK_A))
	{
		object3d->GetTransform().translate.x -= 0.1f;
	}
	if (input->PushKey(DIK_W))
	{
		object3d->GetTransform().translate.y += 0.1f;
	}
	if (input->PushKey(DIK_S))
	{
		object3d->GetTransform().translate.y -= 0.1f;
	}

	backSpritePos1 -= 1.0f;
	backSpritePos2 -= 1.0f;
	if (backSpritePos1 < -1280) {
		backSpritePos1 = 1280;
	}
	if (backSpritePos2 < -1280) {
		backSpritePos2 = 1280;
	}
	backSprite1->SetPosition({ backSpritePos1, 0.0f });
	backSprite2->SetPosition({ backSpritePos2, 0.0f });

	backSprite1->Update();
	backSprite2->Update();

	GreenHpSize--;
	if (GreenHpSize < 0) {
		GreenHpSize = defaultGreenHpSize;
	}
	GreenHpSprite->SetSize({ GreenHpSize, 50.0f });

	RedHpSprite->Update();
	GreenHpSprite->Update();

	number++;
	int32_t workNumber = number;
	int32_t digit = 10000;
	for (int i = 0; i < 5; i++) {
		int nowNumber = workNumber / digit;
		sprites[i]->SetTextureLeftTop({ numberSize.x * nowNumber, 0.0f });
		workNumber %= digit;
		digit /= 10;
	}

	for (uint32_t i = 0; i < 5; i++) {
		sprites[i]->Update();
	}

	object3d->Update();
	//object3d2->Update();

	//particleEmitter.Update();

#pragma endregion


	titleScene->Update();
	gameScene->Update();

#pragma region ImGuiUpdate
	imguiManager->Begin();

#ifdef USE_IMGUI
	//ImGui::ShowDemoWindow();
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

	//ImGui::Begin("Model");
	//ImGui::Checkbox("Draw", &isModel);
	//ImGui::DragFloat3("ModelPosition", &object3d->GetTransform().translate.x, 0.01f, -10.0f, 10.0f);
	//ImGui::DragFloat3("ModelRotate", &object3d->GetTransform().rotate.x, 0.01f, -10.0f, 10.0f);
	//ImGui::DragFloat3("ModelScale", &object3d->GetTransform().scale.x, 0.01f, -10.0f, 10.0f);
	//ImGui::End();

	ImGui::Begin("Sprite");
	//ImGui::Checkbox("Draw", &isSprite);
	Vector2 position = backSprite1->GetPosition();
	ImGui::DragFloat2("SpritePosition", &position.x, 1.0f, -100.0f, 1000.0f, "%.1f");
	backSprite1->SetPosition(position);
	float rotation = backSprite1->GetRotation();
	ImGui::DragFloat("SpriteRotate", &rotation, 0.01f, 10.0f, 10.0f);
	backSprite1->SetRotation(rotation);
	Vector2 size = backSprite1->GetSize();
	ImGui::DragFloat2("SpriteSize", &size.x, 1.0f, 0.0f, 1000.0f);
	backSprite1->SetSize(size);
	Vector4 color = backSprite1->GetColor();
	ImGui::ColorEdit4("SpriteColor", &color.x);
	backSprite1->SetColor(color);
	ImGui::End();

	//ImGui::Begin("Particle");
	//bool useBillBoard = ParticleManager::GetInstance()->GetUseBillBoard();
	//ImGui::Checkbox("BillBoard", &useBillBoard);
	//ParticleManager::GetInstance()->SetUseBillBoard(useBillBoard);
	//bool applyField = ParticleManager::GetInstance()->GetApplyField();
	//ImGui::Checkbox("ApplyField", &applyField);
	//ParticleManager::GetInstance()->SetApplyField(applyField);
	//ImGui::End();
#endif

	imguiManager->End();
#pragma endregion
}

void MyGame::Draw()
{
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

	titleScene->Draw();
	gameScene->Draw();

	backSprite1->Draw();
	backSprite2->Draw();

	for (uint32_t i = 0; i < 5; i++) {
		sprites[i]->Draw();
	}

	RedHpSprite->Draw();
	GreenHpSprite->Draw();

	object3d->Draw();

#pragma endregion

#pragma region ImGuiDraw
	imguiManager->Draw();

#pragma endregion

#pragma region PostDraw
	dxCommon->PostDraw();

#pragma endregion
}
