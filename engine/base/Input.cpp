#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
//初期化
void Input::Initialize(WinApp* winApp)
{
    this->winApp_ = winApp;
    HRESULT result;

    // DirectInputの初期化
   
    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    // キーボードデバイスの生成
    
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    assert(SUCCEEDED(result));
    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));
	
}
//更新
void Input::Update()
{
    //前回のキー入力を保持
    memcpy(keyPre, key, sizeof(key));

    // キーボード情報の取得開始
    keyboard->Acquire();
    
    keyboard->GetDeviceState(sizeof(key), key);
}
//キーの押下をチェック
bool Input::Pushkey(BYTE keyNumber) 
{
    //指定キーを押していればtrueを返す
    if (key[keyNumber]) 
    {
        return true;
    }
    return false;
}
//キーのトリガーをチェック
bool Input::TriggerKey(BYTE keyNumber) 
{
    if (keyPre[keyNumber] == false && key[keyNumber])
    {
        return true;
    }
    return false;
}