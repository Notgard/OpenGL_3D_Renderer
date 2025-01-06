#ifndef __SETUP_H__
#define __SETUP_H__

//includes
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Dear ImGui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "functions.h"
#include "imgui-style.h"

//utilities
std::string read_shader(std::string current_path, int vertex_type);
static void glfw_error_callback(int error, const char *description);
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//initializations
bool initializeGLFW(GLFWwindow **glfWindow);
bool initializeGLAD();
bool initializeDearImGui(GLFWwindow *glfWindow);

int initAll(GLFWwindow **glfWindow);

//clean up
void teardown(GLFWwindow *glfWindow, unsigned int shaderProgram, unsigned int VBO, unsigned int VAO);

#endif // !__SETUP_H__