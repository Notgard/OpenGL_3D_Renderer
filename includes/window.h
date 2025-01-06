#pragma once

#include <string>

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Dear ImGui
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Window
{
public:
    virtual void *get_native_window() = 0;
    virtual void set_native_window(void *window) = 0;
    virtual void set_window_size(int width, int height) = 0;
    virtual void on_scroll(double delta) = 0;
    virtual void on_key(int key, int scancode, int action, int mods) = 0;
    virtual void on_resize(int width, int height) = 0;
    virtual void on_close() = 0;

    int width;
    int height;
    std::string title;
};