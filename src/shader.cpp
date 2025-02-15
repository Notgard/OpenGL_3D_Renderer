#include "shader.h"

#include "config.h"

bool Shader::compile_and_load(const char *vertexShaderSource, const char *fragmentShaderSource, const char *geometryShaderSource)
{
    bool geometryShader = geometryShaderSource != nullptr;

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return false;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return false;
    }

    // geometry shader
    if (geometryShader)
    {
        std::cout << "Compiling geometry shader" << std::endl;
        unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        // check for shader compile errors
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
            return false;
        }
    }

    // load the shader program

    // link shaders
    this->shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if(geometryShader)
    {
        std::cout << "Attaching geometry shader" << std::endl;
        glAttachShader(shaderProgram, geometryShader);
    }
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryShader)
    {
        glDeleteShader(geometryShader);
    }

    return true;
}

void Shader::use()
{
    glUseProgram(shaderProgram);
}

void Shader::unload()
{
    glDeleteProgram(shaderProgram);
}

std::string Shader::read_shader(std::string current_path, int vertex_type)
{
    // reads a shader from a txt file and returns it as a string
    std::string shader_path = current_path + SHADER_FOLDER;

    switch (vertex_type)
    {
    case VERTEX_SHADER:
        shader_path += VERTEX_SHADER_NAME;
        break;
    case FRAGMENT_SHADER:
        shader_path += FRAGMENT_SHADER_NAME;
        break;
    case GEOMETRY_SHADER:
        shader_path += GEOMETRY_SHADER_NAME;
        break;
    default:
        return NULL;
    }

    std::ifstream shader(shader_path);
    std::string shader_code = "";

    if (shader.is_open())
    {
        std::string line;
        while (std::getline(shader, line))
        {
            shader_code += line + "\n";
        }
        shader.close();
    }
    else
    {
        std::cerr << "[ERROR] Couldn't open shader file: " << shader_path << std::endl;
    }

    return shader_code;
}

std::string Shader::read_shader(std::string current_path, std::string shader_name)
{
    // reads a shader from a txt file and returns it as a string
    std::string shader_path = current_path + SHADER_FOLDER;

    shader_path += shader_name;

    std::ifstream shader(shader_path);
    std::string shader_code = "";

    if (shader.is_open())
    {
        std::string line;
        while (std::getline(shader, line))
        {
            shader_code += line + "\n";
        }
        shader.close();
    }
    else
    {
        std::cerr << "[ERROR] Couldn't open shader file: " << shader_path << std::endl;
    }

    return shader_code;
}

void Shader::set_model(const glm::mat4 &model)
{
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::set_view(const glm::mat4 &view)
{
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void Shader::set_projection(const glm::mat4 &projection)
{
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &value)
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::set_vec4(const std::string &name, const glm::vec4 &value)
{
    glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::set_float(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::set_int(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}