#include "D3DResourceLeakChecker.h"
#include "MyGame.h"

#pragma comment(lib,"dxcompiler.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakCheck;

	MyGame game;

	game.Initialize();

	while (true) {
		game.Update();

		if (game.IsEndRequest()) {
			break;
		}

		game.Draw();

	}

	game.Finalize();

	return 0;
}
