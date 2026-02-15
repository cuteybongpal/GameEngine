#include "MenuBar.h"
#include "engine.h"


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