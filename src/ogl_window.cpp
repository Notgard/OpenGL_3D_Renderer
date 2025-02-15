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

    if (cmd_arguments.size() > 0)
    {
        sceneView = new ui::SceneView(cmd_arguments);
    }
    else
    {
        sceneView = new ui::SceneView();
    }
    menuBar = new ui::MenuBarComponent();

    return isRunning;
}

void OGLWindow::render()
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    fps = 1.0 / deltaTime;

    sceneView->set_fps(fps);

    //std::cout << "FPS: " << fps << std::endl;

    Camera *camera = sceneView->get_camera();
    inputHandler->process_camera_movement(camera, deltaTime);

    //std::cout << "Pre-render" << std::endl;
    ogl->pre_render();
    ui->pre_render();

    //std::cout << "Render" << std::endl;

    menuBar->render(sceneView);
    sceneView->render(currentFrame);

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

    if (menuBar->available_input())
    {
        return;
    }

    if (!sceneView->available_input())
    {
        return;
    }

    // Check if any mouse button is pressed
    bool leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool rightPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    bool middlePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

    if (leftPressed || rightPressed || middlePressed)
    {
        if (mousePressed) // Only move if the mouse has changed position
        {
            if (x != lastX || y != lastY)
            {
                if (leftPressed)
                    sceneView->on_mouse_move(x, y, MouseButtons::LEFT_CLICK);
                else if (rightPressed)
                    sceneView->on_mouse_move(x, y, MouseButtons::RIGHT_CLICK);
                else if (middlePressed)
                    sceneView->on_mouse_move(x, y, MouseButtons::MIDDLE_CLICK);
            }
        }
        mousePressed = true;
    }
    else
    {
        mousePressed = false; //reset
    }

    lastX = x;
    lastY = y;
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
    if (menuBar->available_input())
    {
        return;
    }
    sceneView->on_mouse_wheel(delta * 0.5f);
    menuBar->update_camera_configuration(sceneView);
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