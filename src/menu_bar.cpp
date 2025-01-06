#include "menu_bar.h"

namespace ui
{
    void MenuBarComponent::render()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Create"))
                {
                }
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                }
                if (ImGui::MenuItem("Save as.."))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}
