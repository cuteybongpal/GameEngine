#include "Console.h"
#include "engine.h"

void Console::Draw()
{
    ImGui::Begin("Console");
    ImGui::Text("Log: Engine Initialized...");
    ImGui::End();
}