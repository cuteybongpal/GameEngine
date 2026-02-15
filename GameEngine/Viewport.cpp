#include "Viewport.h"
#include "engine.h"

void Viewport::Draw()
{
    ImGui::Begin("Viewport");
    ImGui::Text("Real-time Rendering View");
    // 나중에 여기에 DX11 텍스처를 꽂을 겁니다.
    ImGui::End();
}