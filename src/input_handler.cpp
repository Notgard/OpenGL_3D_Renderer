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

    //handle wasd and arrow key movement
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        break;
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
        if (action == GLFW_PRESS)
        {
            camera->move(CameraMovement::FORWARD);
        }
        break;
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS)
        {
            camera->move(CameraMovement::LEFT);
        }
        break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
        if (action == GLFW_PRESS)
        {
            camera->move(CameraMovement::BACKWARD);
        }
        break;
    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS)
        {
            camera->move(CameraMovement::RIGHT);
        }
        break;
    case GLFW_KEY_Q:
        if (action == GLFW_PRESS)
        {
            camera->move(CameraMovement::UP);
        }
        break;
    case GLFW_KEY_E:
        if (action == GLFW_PRESS)
        {
            camera->move(CameraMovement::DOWN);
        }
        break;
    default:
        break;
    }
}