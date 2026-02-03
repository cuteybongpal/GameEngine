#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>

// 라이브러리 링크 (컴파일러에게 DX11 기능을 쓰겠다고 알림)
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// [1] 전역 변수: 프로그램 어디서든 접근해야 하는 DX11 핵심 객체들
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// [2] 전방 선언: 함수가 아래에 있음을 알림
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 헬퍼 함수: 화면에 그릴 도화지(RenderTargetView) 생성
void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

// 헬퍼 함수: 자원 해제
void CleanupDeviceD3D() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

int main(int, char**)
{
    // 1. Win32 창 클래스 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGuiEngine"), NULL };
    RegisterClassEx(&wc);

    // 2. 창 생성 (HWND 획득)
    HWND hwnd = CreateWindow(wc.lpszClassName, _T("Gemini Game Engine UI"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // 3. DirectX 11 초기화 설정
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    // DX11 장치 및 스왑 체인 생성
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return 1;

    CreateRenderTarget();

    // 4. 창 보이기
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // 5. ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // 도킹 활성화

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // 6. 메인 루프 (심장 박동)
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done) break;

        // ImGui 프레임 시작
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // [여기에 당신의 엔진 UI 코드를 작성하세요]
        {
            ImGui::Begin("Engine Inspector");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            static float f = 0.0f;
            ImGui::SliderFloat("Float Value", &f, 0.0f, 1.0f);

            if (ImGui::Button("Spawn Cube")) {
                // 버튼 클릭 로직
            }
            ImGui::End();
        }

        // 렌더링
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.15f, 0.15f, 0.15f, 1.00f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // V-Sync 사용
    }

    // 7. 종료 처리
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// 창 메시지 핸들러
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}