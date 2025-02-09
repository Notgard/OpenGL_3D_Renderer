#include "menu_bar.h"

namespace ui
{
    void MenuBarComponent::render(SceneView *sceneView)
    {
/*         if (ImGui::BeginMainMenuBar())
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
        } */

        bool window_is_active = false;
        Camera * camera = sceneView->get_camera();

        //std::cout << "Rendering MenuBar" << std::endl;

        //rendering info
        ImGui::Begin("Rendering Info");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) {
            window_is_active = true;
        }
        ImGui::Text("Rendering Info");
        ImGui::Separator();
        //std::cout << "Rendering MenuBar1" << std::endl;
        ImGui::Text("ImGui FPS: %.1f", ImGui::GetIO().Framerate);
        //std::cout << "Rendering MenuBar2" << std::endl;
        ImGui::Text("OpenGL FPS: %.1f", sceneView->get_fps());
        //std::cout << "Rendering MenuBar3" << std::endl;
        ImGui::Text("Camera Position: %s", glm::to_string(camera->get_position()).c_str());
        //std::cout << "Rendering MenuBar4" << std::endl;
        ImGui::Text("Polygon Count: %.0f", sceneView->get_polygon_count());
        //std::cout << "Rendering MenuBar5" << std::endl;
        ImGui::End();
        
        //std::cout << "Rendering MenuBar" << std::endl;

        //side hud menu for scene interaction
        ImGui::Begin("Scene Menu");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) {
            window_is_active = true;
        }
        ImGui::Text("Scene Settings");
        if (ImGui::Checkbox("Enable Grid", &config->enable_grid)) sceneView->set_enable_grid(config->enable_grid);
        if (ImGui::Checkbox("Enable VSync", &config->enable_vsync)) {
            if (config->enable_vsync) {
                glfwSwapInterval(VSYNC_ENABLED);
            } else {
                glfwSwapInterval(VSYNC_DISABLED);
            }
        }
        //std::cout << "Rendering MenuBar" << std::endl;
        if (ImGui::Checkbox("Show Gizmo", &config->enable_gizmo)) sceneView->toggle_gizmo();
        ImGui::Separator();
        ImGui::Text("Camera Settings");
        if (ImGui::Checkbox("Free Camera", &config->free_camera)) camera_load_configuration(camera);
        if (ImGui::SliderFloat("FOV", &config->camera_fov, 1.0f, 90.0f)) camera_load_configuration(camera);
        if (ImGui::SliderFloat("Near", &config->camera_near, 0.1f, 100.0f)) camera_load_configuration(camera);
        if (ImGui::SliderFloat("Far", &config->camera_far, 100.0f, 10000.0f)) camera_load_configuration(camera);
        if (ImGui::SliderFloat("Move Speed", &config->camera_move_speed, 0.1f, 50.0f)) camera_load_configuration(camera);
        //std::cout << "Rendering MenuBar" << std::endl;
        ImGui::Separator();
        ImGui::Text("Light Settings");
        if (ImGui::SliderFloat("Light X", &config->light_position.x, -100.0f, 100.0f)) light_load_configuration(sceneView->get_lights());
        if (ImGui::SliderFloat("Light Y", &config->light_position.y, -100.0f, 100.0f)) light_load_configuration(sceneView->get_lights());
        if (ImGui::SliderFloat("Light Z", &config->light_position.z, -100.0f, 100.0f)) light_load_configuration(sceneView->get_lights());
        ImGui::End();
        //std::cout << "Rendering MenuBar" << std::endl;
        //light color
        ImGui::Begin("Light Color");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) {
            window_is_active = true;
        }
        ImGui::Text("Light Color");
        ImGui::Separator();
        if (ImGui::ColorEdit3("Light Color", config->light_color)) light_load_configuration(sceneView->get_lights());
        if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
            window_is_active = true;
        }
        ImGui::End();
        //std::cout << "Rendering MenuBar" << std::endl;
        //enable texture mapping
        ImGui::Begin("Texture Mapping");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) {
            window_is_active = true;
        }
        ImGui::Text("Texture Mapping");
        ImGui::Separator();
        ImGui::Text("Texture Settings");
        if(ImGui::Checkbox("Enable Texture Mapping", &config->texture_mapping)) shader_load_configuration(sceneView->get_shader());
        ImGui::End();

        //std::cout << "Window is active: " << window_is_active << std::endl;

        enable_input = window_is_active;
    }

    void MenuBarComponent::camera_load_configuration(Camera * camera)
    {
        camera->load_config(config);
    }

    void MenuBarComponent::light_load_configuration(std::vector<Light *> lights)
    {
        for (auto &light : lights)
        {
            light->load_config(config);
        }
    }

    void MenuBarComponent::shader_load_configuration(Shader * shader)
    {
        shader->set_int("textureMapping", config->texture_mapping);
    }

    void MenuBarComponent::update_camera_configuration(SceneView *sceneView)
    {
        Camera * camera = sceneView->get_camera();

        config->camera_fov = glm::degrees(camera->get_fov());
        config->camera_near = camera->get_near();
        config->camera_far = camera->get_far();
        config->free_camera = camera->is_free_camera();

        this->render(sceneView);
    }
}
