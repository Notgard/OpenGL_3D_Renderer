#pragma once

#include "shader.h"
#include "buffer.h"
#include "mesh.h"
#include "camera.h"
#include "light.h"
#include "object.h"

#include "config.h"

// Dear ImGui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "functions.h"
#include "imgui-style.h"

#include <tuple>
#include <memory>

namespace ui
{
    class SceneView
    {
    public:
        SceneView();

        SceneView(std::vector<std::string> cmd_arguments);

        ~SceneView()
        {
            shader->unload();
            frameBuffer->delete_buffers();
            vertexBuffer->delete_buffers();
            safe_delete(shader);
            safe_delete(vertexBuffer);
            safe_delete(frameBuffer);
            safe_delete(meshHandler);
            //safe_delete(objectHandler);
            safe_delete(camera);
            // delete lights
            for (auto light : lights)
            {
                safe_delete(light);
            }
        };

        std::tuple<std::vector<float>, std::vector<unsigned int>> init_scene_vectors();

        void resize(int width, int height);

        void render();

        void on_mouse_wheel(double delta);

        void on_mouse_move(double x, double y, MouseButtons button);

        Camera *get_camera() { return camera; }

        bool available_input() { return enable_input; }

        std::vector<Light *> get_lights() { return lights; }

        Shader *get_shader() { return shader; }

        void set_fps(double fps) { this->fps = fps; }
        double get_fps() { return fps; }

        double get_polygon_count()
        { 
            return objectHandler->get_polygon_count();
        }

        bool get_enable_grid() { return enable_grid; }
        void set_enable_grid(bool enable) { enable_grid = enable; }

        bool get_enable_gizmo() { return enable_gizmo; }
        void toggle_gizmo()
        {
            //enable_gizmo = !enable_gizmo;
            //objectHandler->toggle_object(GIZMO_INDEX, enable_gizmo);
            objectHandler->toggle_object(GIZMO_INDEX);
        }

    private:
        int width;
        int height;
        Shader *shader;
        renderer::OGLFrameBuffer *frameBuffer;
        renderer::OGLVertexBuffer *vertexBuffer;
        MeshHandler *meshHandler;
        //ObjectHandler *objectHandler;
        std::unique_ptr<ObjectHandler> objectHandler;

        Camera *camera;
        std::vector<Light *> lights;
        bool enable_input = true;
        bool enable_grid = true;
        bool enable_gizmo = true;
        double fps;
    };
}
