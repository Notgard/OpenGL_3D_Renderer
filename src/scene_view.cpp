#include "scene_view.h"

namespace ui
{
    SceneView::SceneView() : width(OGL_SCENE_WIDTH), height(OGL_SCENE_HEIGHT)
    {
        // create buffers
        frameBuffer = new renderer::OGLFrameBuffer();
        frameBuffer->create_buffers(width, height);

        vertexBuffer = new renderer::OGLVertexBuffer();
        auto [vertices, indices] = init_scene_vectors();

        vertexBuffer->set_color(true);
        vertexBuffer->create_buffers(vertices, indices);

        shader = new Shader();

        std::filesystem::path currentPath = std::filesystem::current_path();
        std::string vertexShader = shader->read_shader(currentPath.string(), VERTEX_SHADER);
        std::string fragmentShader = shader->read_shader(currentPath.string(), FRAGMENT_SHADER);
        shader->compile_and_load(vertexShader.c_str(), fragmentShader.c_str());

        shader->set_int("textureMapping", 0);

        bool res;
        //ObjectHandler *objectHandler = new ObjectHandler();
        objectHandler = std::make_unique<ObjectHandler>();
        std::cout << "Loading object" << std::endl;
        res = objectHandler->load_object("models/xyz.obj");
        //meshHandler = new MeshHandler();
        //meshHandler->load_mesh("models/xyz.obj");
        if (!res)
        {
            std::cerr << "Failed to load object" << std::endl;
        }
        std::cout << "Total polygon count: " << objectHandler->get_polygon_count() << std::endl;

        camera = new Camera();

        Light *light = new Light();
        lights.push_back(light);
    };

    SceneView::SceneView(std::vector<std::string> cmd_arguments) : width(OGL_SCENE_WIDTH), height(OGL_SCENE_HEIGHT)
    {
        // create buffers
        frameBuffer = new renderer::OGLFrameBuffer();
        frameBuffer->create_buffers(width, height);

        vertexBuffer = new renderer::OGLVertexBuffer();
        auto [vertices, indices] = init_scene_vectors();

        vertexBuffer->set_color(true);
        vertexBuffer->create_buffers(vertices, indices);

        shader = new Shader();

        std::filesystem::path currentPath = std::filesystem::current_path();
        std::string vertexShader = shader->read_shader(currentPath.string(), VERTEX_SHADER);
        std::string fragmentShader = shader->read_shader(currentPath.string(), FRAGMENT_SHADER);
        shader->compile_and_load(vertexShader.c_str(), fragmentShader.c_str());

        shader->set_int("textureMapping", 0);

        objectHandler = std::make_unique<ObjectHandler>();
        //ObjectHandler *objectHandler = new ObjectHandler();
        //meshHandler = new MeshHandler();
        // if mesh file is provided in cmd args, use it, otherwise use default teapot mesh
        if (cmd_arguments.size() > 0)
        {
            //meshHandler->load_mesh(cmd_arguments[0]);
            objectHandler->load_object(cmd_arguments[0]);
        }
        else
        {
            objectHandler->load_object("models/xyz.obj");
            //meshHandler->load_mesh("models/xyz.obj");
        }

        camera = new Camera();

        Light *light = new Light();
        lights.push_back(light);
    };

    std::tuple<std::vector<float>, std::vector<unsigned int>> SceneView::init_scene_vectors()
    {
/*         // Axis length
        float axis_length = 6.0f;

        // XYZ Axes vertices
        std::vector<float> vertices = {
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

        // XYZ Axes indices
        std::vector<unsigned int> indices = {
            0, 1, // X-axis
            2, 3, // Y-axis
            4, 5  // Z-axis
        }; */
        
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Add Grid Lines (XZ Plane)
        float grid_size = 10.0f;
        float grid_spacing = 1.0f;
        //unsigned int index_offset = vertices.size() / 7; // Offset for indices
        unsigned int index_offset = 0; // Offset for indices

        // Grid lines along the X-axis
        for (float z = -grid_size; z <= grid_size; z += grid_spacing)
        {
            vertices.insert(vertices.end(), {
                                                -grid_size, 0.0f, z, 0.5f, 0.5f, 0.5f, 1.0f, // Start of line
                                                grid_size, 0.0f, z, 0.5f, 0.5f, 0.5f, 1.0f   // End of line
                                            });

            indices.push_back(index_offset);
            indices.push_back(index_offset + 1);
            index_offset += 2;
        }

        // Grid lines along the Z-axis
        for (float x = -grid_size; x <= grid_size; x += grid_spacing)
        {
            vertices.insert(vertices.end(), {
                                                x, 0.0f, -grid_size, 0.5f, 0.5f, 0.5f, 1.0f, // Start of line
                                                x, 0.0f, grid_size, 0.5f, 0.5f, 0.5f, 1.0f   // End of line
                                            });

            indices.push_back(index_offset);
            indices.push_back(index_offset + 1);
            index_offset += 2;
        }
        return std::make_tuple(vertices, indices);
    }

    void SceneView::resize(int width, int height)
    {
        this->width = width;
        this->height = height;
        frameBuffer->create_buffers(width, height);
    }

    void SceneView::render()
    {
        //std::cout << "Rendering SceneView" << std::endl;
        unsigned int VAO, VBO;
        // std::tie(VAO, VBO) = dummy_buffer();

        shader->use();

        for (auto &light : lights)
        {
            light->update(shader);
        }

        // framebuffer binding
        frameBuffer->bind();

        // Set the viewport and clear the framebuffer
        // glViewport(0, 0, width, height);
        //glClearColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind and draw
        // vertexBuffer->draw(3);
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        if(enable_grid)
        {
            vertexBuffer->draw(GL_LINES, vertexBuffer->get_index_count());
        }
        //meshHandler->render(shader);
        objectHandler->render(shader);

        frameBuffer->unbind();

        ImGui::Begin("Viewport");

        auto &io = ImGui::GetIO();
        if (io.WantCaptureMouse)
        {
            enable_input = true;
        }
        else
        {
            enable_input = false;
        }

        ImVec2 size = ImGui::GetContentRegionAvail();
        width = size.x;
        height = size.y;

        camera->set_aspect_ratio(size.x / size.y);
        camera->update(shader);

        // adding the render texture to the scene view
        int textureID = frameBuffer->get_texture();
        ImGui::Image((ImTextureID)(intptr_t)textureID, size, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }

    void SceneView::on_mouse_wheel(double delta)
    {
        camera->on_mouse_wheel(delta);
    }

    void SceneView::on_mouse_move(double x, double y, MouseButtons button)
    {
        camera->on_mouse_move(x, y, width, height, button);
    }
}