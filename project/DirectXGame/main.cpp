#include "MyGame.h"

#pragma comment(lib,"dxcompiler.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Framework* game = new MyGame;

	game->Run();

	delete game;

	return 0;
}
