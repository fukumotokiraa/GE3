#include "Framework.h"

#include "D3DResourceLeakChecker.h"

Framework::Framework()
{
}

void Framework::Initialize()
{
}

void Framework::Finalize()
{
}

void Framework::Update()
{
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
