#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>
#include "EditorUI.h"

// 라이브러리 링크: DirectX 11 사용을 위해 필요한 라이브러리들을 링커에 전달
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// [1] 전역 변수 영역
// 프로그램 종료 시까지 유지되어야 하며, 초기화 및 렌더링 루프에서 계속 사용됨
static ID3D11Device* g_pd3dDevice = nullptr;         // GPU 자원(메모리) 생성 담당
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;  // GPU 렌더링 명령 실행 담당
static IDXGISwapChain* g_pSwapChain = nullptr;         // 그려진 그림을 화면으로 전달(교체) 담당
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr; // 최종적으로 그려질 '도화지' 객체

//EditorUI
MenuBar* menuBar = nullptr;


// [2] 함수 전방 선언
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 헬퍼 함수: 스왑 체인으로부터 백 버퍼를 가져와 렌더 타겟(도화지) 생성
void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    // 스왑 체인의 0번 버퍼(백 버퍼)를 텍스처 객체로 가져옴
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    // 가져온 텍스처를 이용해 렌더 타겟 뷰 생성
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release(); // 사용 완료한 인터페이스 참조 해제
}

// 헬퍼 함수: 프로그램 종료 시 할당했던 모든 GPU 자원 해제 (메모리 누수 방지)
void CleanupDeviceD3D() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

int main(int, char**)
{
    // --- (1) Win32 윈도우 생성 단계 ---
    // 윈도우 클래스 설정: OS에 내가 만들 창의 '종류'를 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGuiEngine"), NULL };
    RegisterClassEx(&wc);

    // 실제 윈도우 생성 (제목, 스타일, 좌표, 크기 설정)
    HWND hwnd = CreateWindow(wc.lpszClassName, _T("Gemini Game Engine UI"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // --- (2) DirectX 11 초기화 단계 ---
    // Swap Chain 상세 설정: 더블 버퍼링, 포맷, 출력 창 연결 등
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;                                 // 백 버퍼 개수 (더블 버퍼링)
    sd.BufferDesc.Width = 0;                            // 0으로 설정하면 창 크기에 자동으로 맞춤
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 일반적인 32비트 색상 포맷
    sd.BufferDesc.RefreshRate.Numerator = 60;           // 주사율 60Hz
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 이 버퍼를 출력 도화지로 사용함
    sd.OutputWindow = hwnd;                             // 그림을 내보낼 윈도우 핸들 연결
    sd.SampleDesc.Count = 1;                            // 안티앨리어싱 설정 (기본 1)
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;                                 // 창 모드 시작
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    // GPU 장치(Device)와 스왑 체인(SwapChain)을 동시에 생성
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return 1;

    // 최종 도화지(Render Target) 생성 함수 호출
    CreateRenderTarget();

    // 생성된 창을 화면에 표시
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // --- (3) ImGui 초기화 단계 ---
    IMGUI_CHECKVERSION();           // 버전 확인
    ImGui::CreateContext();         // ImGui 컨텍스트 생성
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // 기능 활성화
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 키보드 네비게이션 사용
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // 창을 붙였다 뗐다 하는 도킹 기능 활성화

    ImGui::StyleColorsDark(); // 다크 모드 테마 적용

    // Win32 및 DX11 백엔드와 ImGui 연결
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // --- (4) 메인 렌더링 루프 ---
    bool done = false;
    while (!done)
    {
        // 윈도우 메시지 처리 (클릭, 닫기, 키 입력 등)
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done) break;

        // [A] ImGui 프레임 시작: 매 프레임 UI를 다시 그릴 준비를 함
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. 전체 화면 도킹 공간 설정
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // 배경 윈도우 스타일 (장식 제거)
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // 메인 베이스 윈도우 시작
        ImGui::Begin("MyDockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        // 도킹 유닛 생성
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        // 여기서 메뉴바 작업 가능

        ImGui::End(); // MyDockSpace 종료
        // 좌측: 계층 구조 (Hierarchy)
        ImGui::Begin("Hierarchy");
        ImGui::Text("Main Scene");
        if (ImGui::TreeNode("Player")) {
            ImGui::BulletText("Camera");
            ImGui::BulletText("Mesh");
            ImGui::TreePop();
        }
        ImGui::End();

        // 중앙: 게임 뷰포트 (실제 게임 화면이 나올 곳)
        ImGui::Begin("Viewport");
        ImGui::Text("Real-time Rendering View");
        // 나중에 여기에 DX11 텍스처를 꽂을 겁니다.
        ImGui::End();

        // 우측: 속성창 (Inspector)
        ImGui::Begin("Inspector");
        ImGui::Text("Object Settings");
        static float color[3] = { 1, 0, 0 };
        ImGui::ColorEdit3("Ambient Color", color);
        ImGui::End();

        // 하단: 콘텐츠 브라우저 / 로그
        ImGui::Begin("Console");
        ImGui::Text("Log: Engine Initialized...");
        ImGui::End();
        // [C] 렌더링 단계
        ImGui::Render(); // ImGui가 설계된 UI를 렌더링 데이터(정점 등)로 변환

        // 배경색 설정 (RGBA)
        const float clear_color_with_alpha[4] = { 0.15f, 0.15f, 0.15f, 1.00f };

        // GPU에게 "이제부터 이 도화지에 그려라"라고 명령
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        // 도화지를 지정된 배경색으로 깨끗하게 지움
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

        // ImGui가 만든 UI 데이터를 실제로 DX11을 통해 화면에 그림
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // 백 버퍼와 프론트 버퍼를 교체하여 모니터에 출력 (수직동기화 적용)
        g_pSwapChain->Present(1, 0);
    }

    // --- (5) 프로그램 종료 처리 ---
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D(); // DX11 자원 해제
    UnregisterClass(wc.lpszClassName, wc.hInstance); // 윈도우 클래스 해제

    return 0;
}

// --- (6) 윈도우 프로시저 (WndProc) ---
// OS에서 발생하는 각종 이벤트(마우스 이동, 클릭, 창 닫기 등)가 여기로 들어옴
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // ImGui가 마우스/키보드 입력을 먼저 가로채서 처리하게 함 (UI 클릭 시 게임이 반응 안 하게)
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) return 0; // Alt 키로 인한 메뉴 진입 차단
        break;
    case WM_DESTROY: // 창의 'X' 버튼을 눌렀을 때
        PostQuitMessage(0); // 루프를 종료하기 위해 QUIT 메시지 보냄
        return 0;
    }
    // 내가 처리하지 않은 나머지 메시지는 OS의 기본 처리 방식에 맡김
    return DefWindowProc(hWnd, msg, wParam, lParam);
}