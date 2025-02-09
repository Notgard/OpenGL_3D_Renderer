#pragma once

#include <map>
#include "camera.h"

struct KeyPress {
    bool current, previous;
};

class InputHandler
{
public:
    InputHandler() {};
    ~InputHandler();
    void input_callback(int key, int scancode, int action, int mods, Camera *camera, GLFWwindow *window);
    void process_camera_movement(Camera *camera, double deltaTime);

    std::map<int, KeyPress> keyPresses;
    std::map<int, bool> keyStates;
};