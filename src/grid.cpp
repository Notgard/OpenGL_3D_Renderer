#include "grid.h"

Grid::Grid()
{
    vertexBuffer = new renderer::OGLVertexBuffer();
    vertexBuffer->set_color(true);
    
    create_grid();
    vertexBuffer->create_buffers(vertices, indices);
}

Grid::~Grid()
{
    vertexBuffer->delete_buffers();
    safe_delete(vertexBuffer);
}

void Grid::create_grid()
{
    unsigned int index_offset = 0;
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
}

void Grid::render(Shader *shader)
{
    //shader->use();
    shader->set_model(model);
    vertexBuffer->draw(GL_LINES, vertexBuffer->get_index_count());
}