#include "render.h"

// Dear ImGui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "functions.h"
#include "imgui-style.h"

namespace renderer
{
    class UIContext : public Render
    {
    public:
        bool init(Window *window) override;
        void pre_render() override;
        void post_render() override;
        void end() override;

    private:
        typedef Render __super;
    };
}