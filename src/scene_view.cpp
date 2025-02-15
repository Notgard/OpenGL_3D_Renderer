#include "scene_view.h"

namespace ui
{
    SceneView::SceneView() : width(OGL_SCENE_WIDTH), height(OGL_SCENE_HEIGHT)
    {
        // create buffers
        frameBuffer = new renderer::OGLFrameBuffer();
        frameBuffer->create_buffers(width, height);

        // vertexBuffer = new renderer::OGLVertexBuffer();
        // auto [vertices, indices] = init_scene_vectors();
        // vertexBuffer->set_color(true);
        // vertexBuffer->create_buffers(vertices, indices);

        grid = std::make_unique<Grid>();

        shader = new Shader();

        std::filesystem::path currentPath = std::filesystem::current_path();
        std::string vertexShader = shader->read_shader(currentPath.string(), VERTEX_SHADER);
        std::string fragmentShader = shader->read_shader(currentPath.string(), FRAGMENT_SHADER);
        std::string geometryShader = shader->read_shader(currentPath.string(), GEOMETRY_SHADER);
        shader->compile_and_load(vertexShader.c_str(), fragmentShader.c_str(), geometryShader.c_str());

        shader->set_int("textureMapping", 0);

        bool res;
        // ObjectHandler *objectHandler = new ObjectHandler();
        objectHandler = std::make_unique<ObjectHandler>();
        std::cout << "Loading object" << std::endl;
        res = objectHandler->load_object("models/xyz.obj");
        // meshHandler = new MeshHandler();
        // meshHandler->load_mesh("models/xyz.obj");
        if (!res)
        {
            std::cerr << "Failed to load object" << std::endl;
        }
        std::cout << "Total polygon count: " << objectHandler->get_polygon_count() << std::endl;

        camera = new Camera();

        // Light *light = new Light();
        lightHandler = std::make_unique<LightHandler>();
        // lights.push_back(light);
    };

    SceneView::SceneView(std::vector<std::string> cmd_arguments) : width(OGL_SCENE_WIDTH), height(OGL_SCENE_HEIGHT)
    {
        // create buffers
        frameBuffer = new renderer::OGLFrameBuffer();
        frameBuffer->create_buffers(width, height);

        // vertexBuffer = new renderer::OGLVertexBuffer();
        //auto [vertices, indices] = init_scene_vectors();
        // vertexBuffer->set_color(true);
        // vertexBuffer->create_buffers(vertices, indices);

        grid = std::make_unique<Grid>();

        shader = new Shader();

        std::filesystem::path currentPath = std::filesystem::current_path();
        std::string vertexShader = shader->read_shader(currentPath.string(), VERTEX_SHADER);
        std::string fragmentShader = shader->read_shader(currentPath.string(), FRAGMENT_SHADER);
        std::string geometryShader = shader->read_shader(currentPath.string(), GEOMETRY_SHADER);
        shader->compile_and_load(vertexShader.c_str(), fragmentShader.c_str(), geometryShader.c_str());

        shader->set_int("textureMapping", 0);

        objectHandler = std::make_unique<ObjectHandler>();
        // ObjectHandler *objectHandler = new ObjectHandler();
        // meshHandler = new MeshHandler();
        //  if mesh file is provided in cmd args, use it
        objectHandler->load_object("models/xyz.obj");
        if (cmd_arguments.size() > 0)
        {
            // meshHandler->load_mesh(cmd_arguments[0]);
            objectHandler->load_object(cmd_arguments[0]);
        }

        camera = new Camera();

        lightHandler = std::make_unique<LightHandler>();
        // Light *light = new Light();
        // lights.push_back(light);
    };

    std::tuple<std::vector<float>, std::vector<unsigned int>> SceneView::init_scene_vectors()
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Add Grid Lines (XZ Plane)
        float grid_size = 10.0f;
        float grid_spacing = 1.0f;
        // unsigned int index_offset = vertices.size() / 7; // Offset for indices
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

    void SceneView::render(double time)
    {
        // std::cout << "Rendering SceneView" << std::endl;
        unsigned int VAO, VBO;
        // std::tie(VAO, VBO) = dummy_buffer();

        lightHandler->update(shader);

        // for (auto &light : lights)
        //{
        //     light->update(shader);
        // }

        // framebuffer binding
        frameBuffer->bind();

        lightHandler->renderLights(camera->get_view_matrix(), camera->get_projection_matrix());

        shader->use();
        shader->set_float("time", time);

        objectHandler->render(shader);

        if (enable_grid)
        {
            grid->render(shader);
            // vertexBuffer->draw(GL_LINES, vertexBuffer->get_index_count());
        }

        // meshHandler->render(shader);

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

    void SceneView::add_random_light() {
        //get random position between -10 and 10
        float x = random_float(-10.0f, 10.0f);
        float y = random_float(-10.0f, 10.0f);
        float z = random_float(-10.0f, 10.0f);
        Light *light = new Light({x, y, z});
        lightHandler->add_light(light);
    }
}