#ifndef __CONFIG_H__
#define __CONFIG_H__

// constants
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define FONT_NAME "includes/JetBrainsMono-ExtraLight.ttf"
#define PROGRAM_NAME "CHIP 8 Emulator"

// shader
#define SHADER_FOLDER "/shaders/"

#define VERTEX_SHADER 0
#define VERTEX_SHADER_NAME "vertex.txt"
#define FRAGMENT_SHADER 1
#define FRAGMENT_SHADER_NAME "fragment.txt"

const float background_color[4] = {0.8f, 0.0f, 0.2f, 1.0f};

// opengl scene
#define OGL_SCENE_WIDTH 800
#define OGL_SCENE_HEIGHT 600

// DPI
#define HIGH_DPI_SCALE_FACTOR 1.0f

// chip 8 emulator
//  Constants for CHIP-8 screen
const int CHIP8_WIDTH = 64;
const int CHIP8_HEIGHT = 32;
const float SCALE_FACTOR = 10.0f; // Scale up the display for visibility

//util functions
template <typename T>
void safe_delete(T *&a)
{
    delete a;
    a = nullptr;
}

#endif // !__CONFIG_H__