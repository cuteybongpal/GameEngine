#include "Hierarchy.h"
#include "engine.h"

void Hierarchy::Draw()
{
    ImGui::Begin("Hierarchy");
    ImGui::Text("Main Scene");
    if (ImGui::TreeNode("Player")) {
        ImGui::BulletText("Camera");
        ImGui::BulletText("Mesh");
        ImGui::TreePop();
    }
    ImGui::End();
}