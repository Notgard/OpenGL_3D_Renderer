#include "input_handler.h"

#include <GLFW/glfw3.h>

InputHandler::~InputHandler()
{
    keyPresses.clear();
}

void InputHandler::input_callback(int key, int scancode, int action, int mods, Camera *camera, GLFWwindow *window)
{
    KeyPress kp;
    kp.current = action;
    kp.previous = keyPresses[key].current;
    keyPresses[key] = kp;

    if (action == GLFW_PRESS)
    {
        keyStates[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        keyStates[key] = false;
    }

    //handle wasd and arrow key movement
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        break;
    case GLFW_KEY_P:
        if (action == GLFW_PRESS)
        {
            camera->print_camera();
        }
        break;
    case GLFW_KEY_F:
        if (action == GLFW_PRESS)
        {
            camera->switch_camera_mode();
            camera->reset();
        }
        break;
    case GLFW_KEY_H:
        if (action == GLFW_PRESS)
        {
            camera->hide();
        }
        break;
    default:
        break;
    }
}

void InputHandler::process_camera_movement(Camera *camera, double deltaTime) {
    float velocity = camera->get_move_speed() * deltaTime;

    if (keyStates[GLFW_KEY_W] || keyStates[GLFW_KEY_UP])
    {
        camera->move(CameraMovement::FORWARD, velocity);
    }
    if (keyStates[GLFW_KEY_S] || keyStates[GLFW_KEY_DOWN])
    {
        camera->move(CameraMovement::BACKWARD, velocity);
    }
    if (keyStates[GLFW_KEY_A] || keyStates[GLFW_KEY_LEFT])
    {
        camera->move(CameraMovement::LEFT, velocity);
    }
    if (keyStates[GLFW_KEY_D] || keyStates[GLFW_KEY_RIGHT])
    {
        camera->move(CameraMovement::RIGHT, velocity);
    }
    if (keyStates[GLFW_KEY_Q] || keyStates[GLFW_KEY_SPACE])
    {
        camera->move(CameraMovement::UP, velocity);
    }
    if (keyStates[GLFW_KEY_E] || keyStates[GLFW_KEY_LEFT_SHIFT])
    {
        camera->move(CameraMovement::DOWN, velocity);
    }
}