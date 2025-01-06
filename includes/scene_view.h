#include "shader.h"
#include "buffer.h"
#include "mesh.h"
#include "camera.h"
#include "light.h"

#include "config.h"

// Dear ImGui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "functions.h"
#include "imgui-style.h"

namespace ui
{
    class SceneView
    {
    public:
        SceneView() : width(OGL_SCENE_WIDTH), height(OGL_SCENE_HEIGHT)
        {
            // create buffers
            frameBuffer = new renderer::OGLFrameBuffer();
            frameBuffer->create_buffers(width, height);

            vertexBuffer = new renderer::OGLVertexBuffer();
            float axis_length = 6.0f;
            std::vector<float> vertices = {
                // Position           // Color (RGBA)
                // X-axis (Red)
                0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,        // Start
                axis_length, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // End

                // Y-axis (Green)
                0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,        // Start
                0.0f, axis_length, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // End

                // Z-axis (Blue)
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,        // Start
                0.0f, 0.0f, axis_length, 0.0f, 0.0f, 1.0f, 1.0f, // End
            };
            std::vector<unsigned int> indices = {
                0, 1, // X-axis
                2, 3, // Y-axis
                4, 5  // Z-axis
            };
            vertexBuffer->set_color(true);
            vertexBuffer->create_buffers(vertices, indices);

            shader = new Shader();

            std::filesystem::path currentPath = std::filesystem::current_path();
            std::string vertexShader = shader->read_shader(currentPath.string(), VERTEX_SHADER);
            std::string fragmentShader = shader->read_shader(currentPath.string(), FRAGMENT_SHADER);
            shader->compile_and_load(vertexShader.c_str(), fragmentShader.c_str());

            meshHandler = new MeshHandler();
            meshHandler->load_mesh("models/teapot.obj");

            camera = new Camera();

            Light *light = new Light();
            lights.push_back(light);
        };

        ~SceneView()
        {
            shader->unload();
            frameBuffer->delete_buffers();
            vertexBuffer->delete_buffers();
            safe_delete(shader);
            safe_delete(vertexBuffer);
            safe_delete(frameBuffer);
            safe_delete(meshHandler);
            safe_delete(camera);
            // delete lights
            for (auto light : lights)
            {
                safe_delete(light);
            }
        };

        void resize(int width, int height);

        void render();

        void on_mouse_wheel(double delta);

        void on_mouse_move(double x, double y, MouseButtons button);

        Camera *get_camera() { return camera; }

        bool available_input() { return enable_input; }

    private:
        int width;
        int height;
        Shader *shader;
        renderer::OGLFrameBuffer *frameBuffer;
        renderer::OGLVertexBuffer *vertexBuffer;
        MeshHandler *meshHandler;
        Camera *camera;
        std::vector<Light *> lights;
        bool enable_input = true;
    };
}
