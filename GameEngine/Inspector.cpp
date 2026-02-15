#include "Inspector.h"
#include "engine.h"

void Inspector::Draw()
{
    ImGui::Begin("Inspector");
    ImGui::Text("Object Settings");
    static float color[3] = { 1, 0, 0 };
    ImGui::ColorEdit3("Ambient Color", color);
    ImGui::End();
}