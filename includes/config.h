#ifndef __CONFIG_H__
#define __CONFIG_H__

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// constants
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define FONT_NAME "includes/JetBrainsMono-ExtraLight.ttf"
#define PROGRAM_NAME "3D Renderer"

// shader
#define SHADER_FOLDER "/shaders/"

#define VERTEX_SHADER 0
#define VERTEX_SHADER_NAME "vertex.vert"
#define FRAGMENT_SHADER 1
#define FRAGMENT_SHADER_NAME "fragment.frag"
#define GEOMETRY_SHADER 2
#define GEOMETRY_SHADER_NAME "geometry.geom"

#define LIGHT_VERTEX_SHADER_NAME "light.vert"
#define LIGHT_FRAGMENT_SHADER_NAME "light.frag"

#define BACKGROUND_R 0.6f
#define BACKGROUND_G 0.6f
#define BACKGROUND_B 0.6f
#define BACKGROUND_A 0.7f

// mesh
#define TEXTURE_DIFFUSE "diffuse_map"
#define TEXTURE_SPECULAR "specular_map"
#define TEXTURE_NORMAL "normal_map"

#define GIZMO_INDEX 0

// opengl scene
#define OGL_SCENE_WIDTH 800
#define OGL_SCENE_HEIGHT 600


#define VSYNC_ENABLED 1
#define VSYNC_DISABLED 0

#define VSYNC VSYNC_ENABLED

#define HIDE_OBJECT 0
#define SHOW_OBJECT 1

// DPI
#define HIGH_DPI_SCALE_FACTOR 1.0f

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

// chip 8 emulator
//  Constants for CHIP-8 screen
const int CHIP8_WIDTH = 64;
const int CHIP8_HEIGHT = 32;
const float SCALE_FACTOR = 10.0f; // Scale up the display for visibility

// util functions
template <typename T>
void safe_delete(T *&a)
{
    delete a;
    a = nullptr;
}

struct Configuration
{
    // shader config
    // std::string shader_folder;
    // std::string vertex_shader_name;
    // std::string fragment_shader_name;
    // scene config
    float c_background_color[4];
    bool texture_mapping;
    // camera configuration
    float camera_fov;
    float camera_near;
    float camera_far;
    float camera_move_speed;
    bool free_camera;
    // light configuration
    glm::vec3 light_position;
    float light_color[3];
    float light_intensity;
    bool enable_grid;
    bool enable_vsync;
    bool enable_gizmo;

    Configuration()
    {
        // shader_folder = SHADER_FOLDER;
        // vertex_shader_name = VERTEX_SHADER_NAME;
        // fragment_shader_name = FRAGMENT_SHADER_NAME;
        c_background_color[0] = BACKGROUND_R;
        c_background_color[1] = BACKGROUND_B;
        c_background_color[2] = BACKGROUND_G;
        c_background_color[3] = BACKGROUND_A;
        texture_mapping = false;
        camera_fov = 51.0f;
        camera_near = 0.1f;
        camera_far = 300.0f;
        free_camera = true;
        camera_move_speed = 3.5f;
        light_position = glm::vec3(-1.5f, 3.5f, 3.0f);
        light_color[0] = 1.0f;
        light_color[1] = 1.0f;
        light_color[2] = 1.0f;
        light_intensity = 100.0f;
        enable_grid = true;
        enable_vsync = VSYNC;
        enable_gizmo = true;
    };
};

#endif // !__CONFIG_H__
