#pragma once
#include <Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"WinApp.h"

class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize(WinApp* winApp);

	void Update();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

private:
	//キーボードデバイス生成
	ComPtr<IDirectInputDevice8>keyboard;
	//全キーの入力情報を取得する
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8>directInput;

	WinApp* winApp_ = nullptr;
};

