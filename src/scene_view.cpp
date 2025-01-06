#include "scene_view.h"

#include <tuple>

namespace ui
{
    static std::tuple<int, int> dummy_buffer()
    {
        unsigned int VAO, VBO;
        // set up vertex data (and buffer(s)) and configure vertex attributes
        float vertices[] =
            {
                -0.5f, -0.5f, 0.0f, // left
                0.5f, -0.5f, 0.0f,  // right
                0.0f, 0.5f, 0.0f    // top
            };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s),
        // and then configure vertex attributes(s)
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO
        // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // you can unbind the VAO afterwards so other VAO calls won't accidentally
        // modify this VAO, but this rarely happens. Modifying other VAOs requires a call
        // to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs)
        // when it's not directly necessary
        glBindVertexArray(0);
        return std::make_tuple(VAO, VBO);
    }

    void SceneView::resize(int width, int height)
    {
        this->width = width;
        this->height = height;
        frameBuffer->create_buffers(width, height);
    }

    void SceneView::render()
    {
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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind and draw
        // vertexBuffer->draw(3);
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        vertexBuffer->draw(GL_LINES, 6);
        meshHandler->render();

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