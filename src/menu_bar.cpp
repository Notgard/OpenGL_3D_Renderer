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
        Camera *camera = sceneView->get_camera();

        // std::cout << "Rendering MenuBar" << std::endl;

        // rendering info
        ImGui::Begin("Rendering Info");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            window_is_active = true;
        }
        ImGui::Text("Rendering Info");
        ImGui::Separator();
        // std::cout << "Rendering MenuBar1" << std::endl;
        ImGui::Text("ImGui FPS: %.1f", ImGui::GetIO().Framerate);
        // std::cout << "Rendering MenuBar2" << std::endl;
        ImGui::Text("OpenGL FPS: %.1f", sceneView->get_fps());
        // std::cout << "Rendering MenuBar3" << std::endl;
        ImGui::Text("Camera Position: %s", glm::to_string(camera->get_position()).c_str());
        // std::cout << "Rendering MenuBar4" << std::endl;
        ImGui::Text("Polygon Count: %.0f", sceneView->get_polygon_count());
        // std::cout << "Rendering MenuBar5" << std::endl;
        ImGui::End();

        // std::cout << "Rendering MenuBar" << std::endl;

        // side hud menu for scene interaction
        ImGui::Begin("Scene Menu");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            window_is_active = true;
        }
        ImGui::Text("Scene Loading");
        // open file dialog
        if (ImGui::Button("Open File"))
        {
            fileDialog.Open();
        }
        fileDialog.Display();
        if (fileDialog.IsOpened())
        {
            window_is_active = true;
        }
        if (fileDialog.HasSelected())
        {
            std::string filename = fileDialog.GetSelected().string();
            std::cout << "Selected filename" << filename << std::endl;
            files.push_back(filename);
            fileDialog.ClearSelected();

            sceneView->add_object(filename);
        }
        ImGui::Separator();
        ImGui::Text("Scene Settings");
        if (ImGui::Checkbox("Enable Grid", &config->enable_grid))
            sceneView->set_enable_grid(config->enable_grid);
        if (ImGui::Checkbox("Enable VSync", &config->enable_vsync))
        {
            if (config->enable_vsync)
            {
                glfwSwapInterval(VSYNC_ENABLED);
            }
            else
            {
                glfwSwapInterval(VSYNC_DISABLED);
            }
        }
        // std::cout << "Rendering MenuBar" << std::endl;
        if (ImGui::Checkbox("Show Gizmo", &config->enable_gizmo))
            sceneView->toggle_gizmo();
        ImGui::Separator();
        ImGui::Text("Camera Settings");
        if (ImGui::Checkbox("Free Camera", &config->free_camera))
            camera_load_configuration(camera);
        if (ImGui::SliderFloat("FOV", &config->camera_fov, 1.0f, 90.0f))
            camera_load_configuration(camera);
        if (ImGui::SliderFloat("Near", &config->camera_near, 0.1f, 100.0f))
            camera_load_configuration(camera);
        if (ImGui::SliderFloat("Far", &config->camera_far, 100.0f, 10000.0f))
            camera_load_configuration(camera);
        if (ImGui::SliderFloat("Move Speed", &config->camera_move_speed, 0.1f, 50.0f))
            camera_load_configuration(camera);
        // std::cout << "Rendering MenuBar" << std::endl;

        ImGui::End();
        // std::cout << "Rendering MenuBar" << std::endl;
        // light color
        ImGui::Begin("Light Settings");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            window_is_active = true;
        }
        ImGui::Text("Add Light");
        if (ImGui::Button("Add Random Light"))
        {
            sceneView->add_random_light();
        }

        ImGui::Text("Light Color");
        ImGui::Separator();
        if (ImGui::ColorEdit3("Light Color", config->light_color))
            light_load_configuration(sceneView->get_light_handler());
        if (ImGui::IsItemActive() || ImGui::IsItemHovered())
        {
            window_is_active = true;
        }
        ImGui::Separator();
        ImGui::Text("Light Position");
        if (ImGui::SliderFloat("Light X", &config->light_position.x, -20.0f, 20.0f))
            light_load_configuration(sceneView->get_light_handler());
        if (ImGui::SliderFloat("Light Y", &config->light_position.y, -20.0f, 20.0f))
            light_load_configuration(sceneView->get_light_handler());
        if (ImGui::SliderFloat("Light Z", &config->light_position.z, -20.0f, 20.0f))
            light_load_configuration(sceneView->get_light_handler());

        ImGui::Separator();
        ImGui::Text("Light Intensity");
        if (ImGui::SliderFloat("Intensity", &config->light_intensity, 0.0f, 1000.0f))
            light_load_configuration(sceneView->get_light_handler());
        ImGui::End();
        // std::cout << "Rendering MenuBar" << std::endl;
        // enable texture mapping
        ImGui::Begin("Texture Mapping");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            window_is_active = true;
        }
        ImGui::Text("Texture Mapping");
        ImGui::Separator();
        ImGui::Text("Texture Settings");
        if (ImGui::Checkbox("Enable Texture Mapping", &config->texture_mapping))
            shader_load_configuration(sceneView->get_shader());
        ImGui::End();

        // add list of loaded objects in the scene that can be selected
        ImGui::Begin("Scene Objects");
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            window_is_active = true;
        }
        ImGui::Text("Scene Objects");
        ImGui::Separator();

        std::vector<std::string> object_names = sceneView->get_object_names();
        std::vector<const char *> items;
        for (const auto &name : object_names)
        {
            items.push_back(name.c_str());
        }

        if (ImGui::ListBox("##ObjectList", &object_index, items.data(), items.size()))
        {
            sceneView->set_selected_object(object_index);
        }

        // selected object size scale
        if(object_index < 0) {
            ImGui::End();
            enable_input = window_is_active;
            return;
        }
        
        std::unique_ptr<Object> &selectedObject = sceneView->get_selected_object(object_index);
        ImGui::Text("Object Scale");
        if (ImGui::SliderFloat("Scale", &selectedObject->current_scale, 0.1f, 10.0f))
        {
            selectedObject->scale(selectedObject->current_scale);
        }

        ImGui::End();

        // std::cout << "Window is active: " << window_is_active << std::endl;

        enable_input = window_is_active;
    }

    void MenuBarComponent::camera_load_configuration(Camera *camera)
    {
        camera->load_config(config);
    }

    void MenuBarComponent::light_load_configuration(std::unique_ptr<LightHandler> &lights)
    {
        lights->load_config(config);
    }

    void MenuBarComponent::shader_load_configuration(Shader *shader)
    {
        std::cout << "Shader Configuration" << std::endl;
        std::cout << "Texture Mapping: " << config->texture_mapping << std::endl;
        // glUniform1i(glGetUniformLocation(shader->get_shader_program(), "textureMapping"), 1);
        shader->set_float("textureMapping", float(config->texture_mapping));
        shader->set_int("textureMappingBool", config->texture_mapping);

        // shader->set_int("textureMapping", config->texture_mapping);
    }

    void MenuBarComponent::update_camera_configuration(SceneView *sceneView)
    {
        Camera *camera = sceneView->get_camera();

        config->camera_fov = glm::degrees(camera->get_fov());
        config->camera_near = camera->get_near();
        config->camera_far = camera->get_far();
        config->free_camera = camera->is_free_camera();

        this->render(sceneView);
    }
}
