#include "WinApp.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SrvManager.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#include "Object3dCommon.h"
#include "Camera.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "D3DResourceLeakChecker.h"

class Framework
{
public:
	Framework();
	virtual ~Framework() = default;

	virtual void Initialize();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	virtual bool IsEndRequest() { return endRequest_; }

	void Run();

protected:
	bool endRequest_ = false;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	SrvManager* srvManager = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	ImGuiManager* imguiManager = nullptr;
	Object3dCommon* object3dCommon = nullptr;
	Camera* camera = nullptr;
};
