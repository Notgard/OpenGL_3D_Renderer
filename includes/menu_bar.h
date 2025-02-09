#pragma once

// Dear ImGui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "functions.h"
#include "imgui-style.h"

#include "config.h"

#include "scene_view.h"

namespace ui
{
    class MenuBarComponent
    {
    public:
        MenuBarComponent() {
            config = new Configuration();
        }

        void render(SceneView *sceneView);

        Configuration *get_config() { return config; }
        bool available_input() { return enable_input; }
        void update_camera_configuration(SceneView *sceneView);

    private:
        Configuration *config;

        void camera_load_configuration(Camera *camera);
        void light_load_configuration(std::vector<Light *> lights);
        void shader_load_configuration(Shader *shader);

        bool enable_input = true;
    };
}