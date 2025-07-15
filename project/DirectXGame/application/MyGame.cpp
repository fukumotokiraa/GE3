#include "MyGame.h"

#include <fstream>
#include <externals/json.hpp>

MyGame::MyGame()
	:particleEmitter(particleGroups, emissionInterval) {
}

MyGame::~MyGame()
{
}

void MyGame::Initialize()
{
	struct ObjectData {
		std::string type;
		std::string name;

		struct Transform {
			Vector3 translation;
			Vector3 rotation;
			Vector3 scaling;
		};

		Transform transform;

		std::string file_name;
	};
	struct LevelData {
		std::string name;
		std::vector<ObjectData> objects;
	};

	Framework::Initialize();


	const std::string fullPath = std::string("resources/levels/") + "scene.json";
	std::ifstream file;
	file.open(fullPath);
	if(file.fail()) {
		assert(0);
	}
	nlohmann::json deserialized;
	file >> deserialized;
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	LevelData* levelData = new LevelData();

	levelData->name = deserialized["name"].get<std::string>();
	assert(levelData->name == "scene");
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));
		if (object["type"].get<std::string>() == "MESH") {
			levelData->objects.emplace_back(ObjectData{});
			ObjectData& objectData = levelData->objects.back();
			objectData.type = object["type"].get<std::string>();
			objectData.type = object["name"].get<std::string>();

			nlohmann::json& transform = object["transform"];
			objectData.transform.translation.x = (float)transform["translation"][0];
			objectData.transform.translation.y = (float)transform["translation"][1];
			objectData.transform.translation.z = (float)transform["translation"][2];

			objectData.transform.rotation.x = -(float)transform["rotation"][0];
			objectData.transform.rotation.y = -(float)transform["rotation"][1];
			objectData.transform.rotation.z = -(float)transform["rotation"][2];

			objectData.transform.scaling.x = (float)transform["scale"][0];
			objectData.transform.scaling.y = (float)transform["scale"][1];
			objectData.transform.scaling.z = (float)transform["scale"][2];

			if (object.contains("file_name")) {
				objectData.file_name = object["file_name"].get<std::string>();
			}
		}
	}

	ModelManager::GetInstance()->LoadModel("Player.gltf");
	//レベルデータからオブジェクトを生成、配置
	for(auto& objectData : levelData->objects) {
		//モデルファイル名 objectData.file_name にあれば入っている → file_name を元に、モデルデータを特定する
		Model* sceneModel = nullptr;
		decltype(models)::iterator it = models.find(objectData.file_name);
		if (it != models.end()) { sceneModel = it->second; }
		//モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = new Object3d;
		newObject->Initialize(object3dCommon, sceneModel);
		newObject->SetModel("Player.gltf");
		newObject->SetPosition(objectData.transform.translation);
		newObject->SetRotation(objectData.transform.rotation);
		newObject->SetScale(objectData.transform.scaling);
		objects.push_back(newObject);
	}

	//for(auto& objectData : levelData->objects) {
	//	Model* sceneModel = nullptr;
	//	decltype(models)::iterator it = models.find(objectData.file_name);
	//	if (it != models.end()) { sceneModel = it->second; }
	//	Object3d* newObject = new Object3d;
	//	newObject->Initialize(object3dCommon, sceneModel);
	//	newObject->SetPosition(objectData.transform.translation);
	//	newObject->SetRotation(objectData.transform.rotation);
	//	newObject->SetScale(objectData.transform.scaling);
	//	objects.push_back(newObject);
	//}


#pragma region 各オブジェクトの初期化

	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	sprite = new Sprite();
	sprite->Initialize(spriteCommon, "resources/uvChecker.png");
	sprite->SetPosition({ 100.0f,100.0f });

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

	ParticleManager::GetInstance()->CreateParticleGroup("example", "resources/circle.png", "plane.obj");
	particle.transform.translate = { 0.0f, 0.0f, 0.0f };
	particleGroup.particles.push_back(particle);
	particleGroups["example"] = particleGroup;

#pragma endregion

}

void MyGame::Finalize()
{
#pragma region Finalize

	ParticleManager::GetInstance()->Finalize();
	imguiManager->Finalize();
	ModelManager::GetInstance()->Finalize();
	winApp->Finalize();

	delete object3d2;
	delete model2;
	delete object3d;
	delete model;
	for (auto& object : objects) {
		delete object;
	}
	TextureManager::GetInstance()->Finalize();
	for (uint32_t i = 0; i < 5; i++) {
		delete sprites[i];
	}
	delete sprite;


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

	sprite->Update();
	for (uint32_t i = 0; i < 5; i++) {
		sprites[i]->Update();
	}

	object3d->Update();
	object3d2->Update();
	for (auto& object : objects) {
		object->Update();
	}

	particleEmitter.Update();

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
	for (auto& object : objects) {
		object->Draw();
	}

	ParticleManager::GetInstance()->Draw();

#pragma endregion

#pragma region ImGuiDraw
	imguiManager->Draw();

#pragma endregion

#pragma region PostDraw
	dxCommon->PostDraw();

#pragma endregion
}
