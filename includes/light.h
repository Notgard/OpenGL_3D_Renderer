#pragma once

#include "shader.h"
#include "buffer.h"

class Light
{
public:
    Light()
    {
        ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
        diffuse = glm::vec4(1.f, 1.0f, 1.f, 1.0f);
        specular = 128.0f;
    };

    Light(glm::vec3 position): position(position) {
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

    void load_config(Configuration *config)
    {
        position = config->light_position;
        color = glm::vec3(config->light_color[0], config->light_color[1], config->light_color[2]);
        intensity = config->light_intensity;
    }

    glm::vec3 get_position()
    {
        return position;
    }

    glm::vec3 get_color()
    {
        return color;
    }

    float get_intensity()
    {
        return intensity;
    }

private:
    glm::vec3 position = glm::vec3(-1.5f, 3.5f, 3.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    float intensity = 100.0f;

    // phong lighting coefficients
    glm::vec4 ambient;
    glm::vec4 diffuse;
    float specular;
};

class LightHandler
{

public:
    LightHandler()
    {
        Light *light = new Light();
        lights.push_back(light);
        lightShader = new Shader();
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::string vertexShader = lightShader->read_shader(currentPath.string(), LIGHT_VERTEX_SHADER_NAME);
        std::string fragmentShader = lightShader->read_shader(currentPath.string(), LIGHT_FRAGMENT_SHADER_NAME);
        lightShader->compile_and_load(vertexShader.c_str(), fragmentShader.c_str());

        create_cube();
    };

    ~LightHandler()
    {
        //delete all lights
        for (auto &light : lights)
        {
            safe_delete(light);
        }
        //delete light shader
        lightShader->unload();
        safe_delete(lightShader);
        //delete light buffer
        if (lightBuffer != nullptr)
        {
            lightBuffer->delete_buffers();
            safe_delete(lightBuffer);
        }
    };

    void update(Shader *shader)
    {
        for (auto &light : lights)
        {
            light->update(shader);
        }
    }

    void load_config(Configuration *config)
    {
        for (auto &light : lights)
        {
            light->load_config(config);
        }
    }

    void add_light(Light *light)
    {
        lights.push_back(light);
    }

    void renderLights(glm::mat4 view, glm::mat4 projection)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        lightShader->use();
        lightShader->set_view(view);
        lightShader->set_projection(projection);

        for (auto &light : lights)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, light->get_position());
            model = glm::scale(model, glm::vec3(0.2f));

            lightShader->set_model(model);
            lightShader->set_vec3("lightColor", light->get_color());

            render_cube();
        }

        glDisable(GL_BLEND);
    }

private:
    std::vector<Light *> lights;
    Shader *lightShader;
    renderer::OGLVertexBuffer *lightBuffer;

    void create_cube() {
        std::vector<float> vertices = {
            // Positions      
           -0.5f, -0.5f, -0.5f,  
            0.5f, -0.5f, -0.5f,  
            0.5f,  0.5f, -0.5f,  
           -0.5f,  0.5f, -0.5f,  
    
           -0.5f, -0.5f,  0.5f,  
            0.5f, -0.5f,  0.5f,  
            0.5f,  0.5f,  0.5f,  
           -0.5f,  0.5f,  0.5f   
        };
    
        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0, // Back
            4, 5, 6, 6, 7, 4, // Front
            0, 1, 5, 5, 4, 0, // Bottom
            2, 3, 7, 7, 6, 2, // Top
            0, 3, 7, 7, 4, 0, // Left
            1, 2, 6, 6, 5, 1  // Right
        };

        lightBuffer = new renderer::OGLVertexBuffer();
        lightBuffer->create_buffers(vertices, indices);
    }

    void render_cube() {
        lightBuffer->draw(GL_TRIANGLES, lightBuffer->get_index_count());
    }
};