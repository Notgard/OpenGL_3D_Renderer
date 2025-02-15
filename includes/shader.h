#ifndef __SHADER_H__
#define __SHADER_H__

//includes
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// build and compile our shader program
//void buildShaderProgram(unsigned int &shaderProgram, unsigned int &VBO, unsigned int &VAO, unsigned int &FBO, const char *vertexShaderSource, const char *fragmentShaderSource);
//void renderToFramebuffer(unsigned int &FBO, unsigned int &VAO, unsigned int &shaderProgram);
//void bindFrameBuffer(unsigned int &FBO, int width, int height);
//void unbindFrameBuffer();

class Shader
{
private:
    unsigned int shaderProgram;

public:
    Shader() = default;

    bool compile_and_load(const char *vertexShaderSource, const char *fragmentShaderSource, const char *geometryShaderSource = nullptr);
    
    std::string read_shader(std::string current_path, int vertex_type);
    std::string read_shader(std::string current_path, std::string shader_name);

    void use();

    void unload();

    unsigned int get_shader_program() const { return shaderProgram; };

    void set_model(const glm::mat4 &model);
    void set_view(const glm::mat4 &view);
    void set_projection(const glm::mat4 &projection);

    void set_vec3(const std::string &name, const glm::vec3 &value);
    void set_vec4(const std::string &name, const glm::vec4 &value);
    void set_float(const std::string &name, float value);
    void set_int(const std::string &name, int value);
};

#endif // !__SHADER_H__