#include "ogl_window.h"

OGLWindow::~OGLWindow()
{
    ui->end();
    ogl->end();

    safe_delete(ui);
    safe_delete(ogl);
    safe_delete(inputHandler);
    safe_delete(sceneView);
    safe_delete(menuBar);
}

bool OGLWindow::init(int width, int height, const std::string &title)
{
    this->width = width;
    this->height = height;
    this->title = title;

    ogl->init(this);
    ui->init(this);

    sceneView = new SceneView();
    menuBar = new MenuBarComponent();

    return isRunning;
}

void OGLWindow::render()
{
    ogl->pre_render();
    ui->pre_render();

    //menuBar->render();
    sceneView->render();

    ui->post_render();
    ogl->post_render();

    input_handler();
}

void OGLWindow::input_handler()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (glfwWindowShouldClose(static_cast<GLFWwindow *>(window)))
    {
        isRunning = false;
    }

    if(!sceneView->available_input()) {
        return;
    }

    if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
        sceneView->on_mouse_move(x, y, MouseButtons::LEFT_CLICK);
    else if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
        sceneView->on_mouse_move(x, y, MouseButtons::RIGHT_CLICK);
    else if (glfwGetMouseButton(window, 2) == GLFW_PRESS)
        sceneView->on_mouse_move(x, y, MouseButtons::MIDDLE_CLICK);
}

void OGLWindow::on_key(int key, int scancode, int action, int mods)
{
    inputHandler->input_callback(key, scancode, action, mods, sceneView->get_camera(), this->window);
}

void OGLWindow::set_window_size(int width, int height)
{
}

void OGLWindow::on_scroll(double delta)
{
    sceneView->on_mouse_wheel(delta * 0.5f);
}

void OGLWindow::on_resize(int width, int height)
{
    this->width = width;
    this->height = height;
    sceneView->resize(width, height);
    this->render();
}

void OGLWindow::on_close()
{
    isRunning = false;
}