#include "MyGame.h"

void MyGame::Initialize()
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
	ModelManager::GetInstance()->LoadModel("axis.obj");
	object3d->SetModel("axis.obj");

	model2 = new Model();
	object3d2 = new Object3d();
	object3d2->Initialize(object3dCommon, model2);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	object3d2->SetModel("plane.obj");

	ParticleManager::GetInstance()->CreateParticleGroup("example", "resources/circle.png", "plane.obj");
	particle.transform.translate = { 0.0f, 0.0f, 0.0f };
	particleGroup.particles.push_back(particle);
	particleGroups["example"] = particleGroup;

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	float emissionInterval = 1.0f;
	ParticleEmitter particleEmitter(randomEngine, particleGroups, emissionInterval);

#pragma endregion

}

void MyGame::Finalize()
{
}

void MyGame::Update()
{
}

void MyGame::Draw()
{
}
