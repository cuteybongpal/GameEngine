#include "MenuBar.h"
#include <imgui.h>
#include <WinUser.h>


void MenuBar::Draw()
{
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) { PostQuitMessage(0); }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}