#include "Viewport.h"
#include "engine.h"

void Viewport::Draw(ID3D11ShaderResourceView* srv)
{
    ImGui::Begin("Viewport");
    ImGui::Text("Real-time Rendering View");
    ImVec2 size = ImGui::GetContentRegionAvail();
    // 나중에 여기에 DX11 텍스처를 꽂기
    ImGui::Image((ImTextureID)srv, size);

    ImGui::End();
}