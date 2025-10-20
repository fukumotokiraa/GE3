#include "MyGame.h"

MyGame::MyGame()
{
}

MyGame::~MyGame()
{
}

void MyGame::Initialize()
{
	Framework::Initialize();

	sceneManager_ = new SceneManager();
	sceneManager_->SetSpriteCommon(spriteCommon);
	sceneManager_->SetObject3dCommon(object3dCommon);
	sceneManager_->SetInput(input);

	BaseScene* scene = new TitleScene();
	//scene->SetSpriteCommon(spriteCommon);
	//scene->SetInput(input);
	sceneManager_->SetNextScene(scene);

#pragma region 各オブジェクトの初期化


	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("resources/swordTop.png");
	TextureManager::GetInstance()->LoadTexture("resources/swordMid.png");
	TextureManager::GetInstance()->LoadTexture("resources/swordBot.png");

	sprite = new Sprite();
	sprite->Initialize(spriteCommon, "resources/uvChecker.png");
	sprite->SetPosition({ 100.0f,100.0f ,-200.0f});

	swordTop = new Sprite();
	swordTop->Initialize(spriteCommon, "resources/swordTop.png");
	swordTop->SetPosition(topStartPos);

	swordMid = new Sprite();
	swordMid->Initialize(spriteCommon, "resources/swordMid.png");
	swordMid->SetPosition(midStartPos);

	swordBot = new Sprite();
	swordBot->Initialize(spriteCommon, "resources/swordBot.png");
	swordBot->SetPosition(botStartPos);

	for (uint32_t i = 0; i < 5; i++) {
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

	model = new Model();
	object3d = new Object3d();
	object3d->Initialize(object3dCommon, model);
	ModelManager::GetInstance()->LoadModel("AnimatedCube.gltf");
	object3d->SetModel("AnimatedCube.gltf");

	model2 = new Model();
	object3d2 = new Object3d();
	object3d2->Initialize(object3dCommon, model2);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	object3d2->SetModel("plane.obj");

	board = new Model();
	boardObject = new Object3d();
	boardObject->Initialize(object3dCommon, board);
	ModelManager::GetInstance()->LoadModel("board.gltf");
	boardObject->SetModel("board.gltf");


#pragma endregion


}

void MyGame::Finalize()
{
#pragma region Finalize

	ParticleManager::GetInstance()->Finalize();
	imguiManager->Finalize();
	ModelManager::GetInstance()->Finalize();
	winApp->Finalize();

	delete boardObject;
	delete board;
	delete object3d2;
	delete model2;
	delete object3d;
	delete model;
	TextureManager::GetInstance()->Finalize();
	for (uint32_t i = 0; i < 5; i++) {
		delete sprites[i];
	}
	delete swordTop;
	delete swordMid;
	delete swordBot;
	delete sprite;
	delete sceneManager_;

#pragma endregion

	Framework::Finalize();
}

void MyGame::Update()
{
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

	if (input->TriggerKey(DIK_SPACE) && spriteMoveState == SpriteMoveState::Idle) {
		spriteMoveState = SpriteMoveState::Entering;
		spriteMoveTimer = 0.0f;
		swordMidState = SpriteMoveState::Idle;
		swordBotState = SpriteMoveState::Idle;
		swordMidTimer = 0.0f;
		swordBotTimer = 0.0f;
	}

	switch (spriteMoveState) {
	case SpriteMoveState::Idle:
		// 何もしない
		break;
	case SpriteMoveState::Entering:
		spriteMoveTimer += 1.0f / 60.0f; // フレームレート固定なら
		{
			float t = std::clamp(spriteMoveTimer / enterDuration, 0.0f, 1.0f);
			float easeT = easeOutCubic(t);
			Vector3 pos = {
				topStartPos.x + (topCenterPos.x - topStartPos.x) * easeT,
				topStartPos.y
			};
			swordTop->SetPosition(pos);
			if (t >= 1.0f) {
				spriteMoveState = SpriteMoveState::Staying;
				spriteMoveTimer = 0.0f;
			}
		}
		break;
	case SpriteMoveState::Staying:
		spriteMoveTimer += 1.0f / 60.0f;
		swordTop->SetPosition(topCenterPos);
		if (spriteMoveTimer >= stayDuration) {
			spriteMoveState = SpriteMoveState::Exiting;
			spriteMoveTimer = 0.0f;
		}
		break;
	case SpriteMoveState::Exiting:
		spriteMoveTimer += 1.0f / 60.0f;
		{
			float t = std::clamp(spriteMoveTimer / exitDuration, 0.0f, 1.0f);
			float easeT = easeOutCubic(t);
			Vector3 pos = {
				topCenterPos.x + (topEndPos.x - topCenterPos.x) * easeT,
				topCenterPos.y
			};
			swordTop->SetPosition(pos);
			if (t >= 1.0f) {
				spriteMoveState = SpriteMoveState::Idle;
				swordTop->SetPosition(topStartPos);
			}
		}
		break;
	}
	switch (swordMidState) {
	case SpriteMoveState::Idle:
		if (spriteMoveState != SpriteMoveState::Idle) {
			swordMidTimer += 1.0f / 60.0f;
			if (swordMidTimer >= swordMidDelay) {
				swordMidState = SpriteMoveState::Entering;
				swordMidTimer = 0.0f;
			}
		}
		break;
	case SpriteMoveState::Entering:
		swordMidTimer += 1.0f / 60.0f;
		{
			float t = std::clamp(swordMidTimer / enterDuration, 0.0f, 1.0f);
			float easeT = easeOutCubic(t);
			Vector3 pos = {
				midStartPos.x + (midCenterPos.x - midStartPos.x) * easeT,
				midStartPos.y
			};
			swordMid->SetPosition(pos);
			if (t >= 1.0f) {
				swordMidState = SpriteMoveState::Staying;
				swordMidTimer = 0.0f;
			}
		}
		break;
	case SpriteMoveState::Staying:
		swordMidTimer += 1.0f / 60.0f;
		swordMid->SetPosition(midCenterPos);
		if (swordMidTimer >= stayDuration) {
			swordMidState = SpriteMoveState::Exiting;
			swordMidTimer = 0.0f;
		}
		break;
	case SpriteMoveState::Exiting:
		swordMidTimer += 1.0f / 60.0f;
		{
			float t = std::clamp(swordMidTimer / exitDuration, 0.0f, 1.0f);
			float easeT = easeOutCubic(t);
			Vector3 pos = {
				midCenterPos.x + (midEndPos.x - midCenterPos.x) * easeT,
				midCenterPos.y
			};
			swordMid->SetPosition(pos);
			if (t >= 1.0f) {
				swordMidState = SpriteMoveState::Idle;
				swordMid->SetPosition(midStartPos);
			}
		}
		break;
	}

	// Botの処理
	switch (swordBotState) {
	case SpriteMoveState::Idle:
		if (spriteMoveState != SpriteMoveState::Idle) {
			swordBotTimer += 1.0f / 60.0f;
			if (swordBotTimer >= swordBotDelay) {
				swordBotState = SpriteMoveState::Entering;
				swordBotTimer = 0.0f;
			}
		}
		break;
	case SpriteMoveState::Entering:
		swordBotTimer += 1.0f / 60.0f;
		{
			float t = std::clamp(swordBotTimer / enterDuration, 0.0f, 1.0f);
			float easeT = easeOutCubic(t);
			Vector3 pos = {
				botStartPos.x + (botCenterPos.x - botStartPos.x) * easeT,
				botStartPos.y
			};
			swordBot->SetPosition(pos);
			if (t >= 1.0f) {
				swordBotState = SpriteMoveState::Staying;
				swordBotTimer = 0.0f;
			}
		}
		break;
	case SpriteMoveState::Staying:
		swordBotTimer += 1.0f / 60.0f;
		swordBot->SetPosition(botCenterPos);
		if (swordBotTimer >= stayDuration) {
			swordBotState = SpriteMoveState::Exiting;
			swordBotTimer = 0.0f;
		}
		break;
	case SpriteMoveState::Exiting:
		swordBotTimer += 1.0f / 60.0f;
		{
			float t = std::clamp(swordBotTimer / exitDuration, 0.0f, 1.0f);
			float easeT = easeOutCubic(t);
			Vector3 pos = {
				botCenterPos.x + (botEndPos.x - botCenterPos.x) * easeT,
				botCenterPos.y
			};
			swordBot->SetPosition(pos);
			if (t >= 1.0f) {
				swordBotState = SpriteMoveState::Idle;
				swordBot->SetPosition(botStartPos);
			}
		}
		break;
	}

	swordTop->Update();
	swordMid->Update();
	swordBot->Update();

	//scene->Update();

	sprite->Update();
	for (uint32_t i = 0; i < 5; i++) {
		sprites[i]->Update();
	}

	object3d->Update();
	object3d2->Update();
	boardObject->Update();

	sceneManager_->Update();

#pragma endregion

	Framework::Update();

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
	ImGui::Checkbox("Draw", &isSprite);
	Vector3 position = sprite->GetPosition();
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

	//scene->Draw();

	swordTop->Draw();
	swordMid->Draw();
	swordBot->Draw();

	if (isSprite) {
		sprite->Draw();
	}

	sceneManager_->Draw();
	//for (uint32_t i = 0; i < 5; i++) {
	//	sprites[i]->Draw();
	//}
	if (isModel) {
		object3d->Draw();
	}
	//boardObject->Draw();

	//object3d2->Draw();

#pragma endregion

#pragma region ImGuiDraw
	imguiManager->Draw();

#pragma endregion

#pragma region PostDraw
	dxCommon->PostDraw();

#pragma endregion
}
