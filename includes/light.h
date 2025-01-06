#pragma once

#include "shader.h"

class Light
{
public:
    Light() : position(glm::vec3(1.5f, 3.5f, 3.0f)), color(glm::vec3(1.0f, 1.0f, 1.0f)), intensity(100.0f)
    {
        ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
        diffuse = glm::vec4(1.f, 1.0f, 1.f, 1.0f);
        specular = 128.0f;
    };
    ~Light() {};

    void update(Shader *shader)
    {
        shader->set_vec3("lightPosition", position);
        shader->set_vec3("lightColor", color);
        shader->set_float("lightIntensity", intensity);

        // phong lighting coefficients
        shader->set_vec4("lightAmbient", ambient);
        shader->set_vec4("lightDiffuse", diffuse);
        shader->set_float("lightSpecular", specular);
    }

private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    // phong lighting coefficients
    glm::vec4 ambient;
    glm::vec4 diffuse;
    float specular;
};