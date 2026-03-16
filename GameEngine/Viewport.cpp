#include "Viewport.h"
#include "engine.h"
#include "EditorCamera.h"

void Viewport::Draw(EditorCamera* editorCamera)
{
    ImGui::Begin("Viewport");
    ImGui::Text("Real-time Rendering View");
    ImVec2 size = ImGui::GetContentRegionAvail();
    editorCamera->Draw();
    // 나중에 여기에 DX11 텍스처를 꽂기
    ImGui::Image((ImTextureID)editorCamera->srv, size);
    ImGui::End();
}