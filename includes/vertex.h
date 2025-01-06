#pragma once

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 tex_coords;
    glm::vec3 normal;
    glm::vec4 color;

    Vertex() {}
    
    Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const glm::vec3 &norm, const glm::vec4 col)
    {
        position = pos;
        tex_coords = tex;
        normal = norm;
        color = col;
    }
};