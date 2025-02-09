#pragma once

#include "window.h"
#include "ui.h"
#include "ogl.h"
#include "scene_view.h"
#include "input_handler.h"
#include "menu_bar.h"

using namespace renderer;
using namespace ui;

class OGLWindow : public Window
{
public:
    OGLWindow() : isRunning(true)
    {
        window = nullptr;
        ui = new UIContext();
        ogl = new OGLRender();
        inputHandler = new InputHandler();
    };
    ~OGLWindow();

    bool init(int width, int height, const std::string &title);

    void *get_native_window() override { return window; }
    void set_native_window(void *window) override { this->window = static_cast<GLFWwindow *>(window); }
    void set_window_size(int width, int height) override;

    void on_scroll(double delta) override;
    void on_key(int key, int scancode, int action, int mods) override;
    void on_resize(int width, int height) override;
    void on_close() override;

    void render();
    void input_handler();

    bool is_running() { return isRunning; }
    
    void set_cmd_arguments(std::vector<std::string> arguments)
    {
        cmd_arguments = arguments;
    }

private:
    GLFWwindow *window;

    UIContext *ui;
    OGLRender *ogl;

    // ui
    ui::SceneView *sceneView;
    ui::MenuBarComponent *menuBar;

    // input
    InputHandler *inputHandler = nullptr;
    std::vector<std::string> cmd_arguments;

    bool isRunning;
    double deltaTime;
    double currentTime;
    double currentFrame;
    double lastFrame;
    double fps;
    double lastX = 0.0, lastY = 0.0;
    bool mousePressed = false;
};
