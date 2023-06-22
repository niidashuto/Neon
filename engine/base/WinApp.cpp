#include "WinApp.h"
#pragma comment(lib,"winmm.lib")
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ウィンドウプロシージャ
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    //ImGui用ウィンドウプロシージャ呼び出し
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;
    //メッセージで分岐
    switch (msg)
    {
    case WM_DESTROY://ウィンドウが破棄された
        PostQuitMessage(0);//OSに対して、アプリの終了を伝える
        return 0;

    }
    return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理を行う
}
//初期化
void WinApp::Initialize()
{
    //システムタイマーの分解能を上げる
    timeBeginPeriod(1);

    // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
    w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);
    
    RECT wrc = { 0,0,window_width,window_height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウオブジェクトの生成
    hwnd = CreateWindow(w.lpszClassName, // クラス名
        L"DirectXGame",         // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
        CW_USEDEFAULT,              // 表示X座標（OSに任せる）
        CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
        wrc.right - wrc.left,       // ウィンドウ横幅
        wrc.bottom - wrc.top,   // ウィンドウ縦幅
        nullptr,                // 親ウィンドウハンドル
        nullptr,                // メニューハンドル
        w.hInstance,            // 呼び出しアプリケーションハンドル
        nullptr);               // オプション

    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);

   
}
//更新
void WinApp::Update()
{

}
//終了
void WinApp::Finalize()
{
    //ウィンドウクラスを登録解除
    UnregisterClass(w.lpszClassName, w.hInstance);
}
//メッセージの処理
bool WinApp::ProcessMessage()
{
    MSG msg{};  // メッセージ

    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (msg.message == WM_QUIT)
    {
        return true;
    }

    return false;
}