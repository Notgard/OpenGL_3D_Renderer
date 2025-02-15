#pragma once

#include "shader.h"
#include "buffer.h"

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

class Grid
{
public:
    Grid();

    ~Grid();

    void render(Shader *shader);

private:
    void create_grid();

    float grid_size = 10.0f;
    float grid_spacing = 1.0f;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translation = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    renderer::OGLVertexBuffer *vertexBuffer;
};
