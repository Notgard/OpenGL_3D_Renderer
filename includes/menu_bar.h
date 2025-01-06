#pragma once

// Dear ImGui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "functions.h"
#include "imgui-style.h"

namespace ui
{
    class MenuBarComponent
    {
    public:
        MenuBarComponent() {};

        void render();
    };
}