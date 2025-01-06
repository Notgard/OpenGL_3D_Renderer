#include "ogl.h"

#include "config.h"

namespace renderer
{
    static void on_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
        pWindow->on_key(key, scancode, action, mods);
    }

    static void on_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        auto pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
        pWindow->on_scroll(yoffset);
    }

    static void on_window_size_callback(GLFWwindow *window, int width, int height)
    {
        auto pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
        pWindow->on_resize(width, height);
    }

    static void on_window_close_callback(GLFWwindow *window)
    {
        Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
        pWindow->on_close();
    }

    static void glfw_error_callback(int error, const char *description)
    {
        std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
    }

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void glMessageCallback(GLenum, GLenum, GLuint, GLenum, GLsizei,
                                  const GLchar *message, const void *)
    {
        glfw_error_callback(0, message);
    }

    // initializes the OpenGL renderer (GLFW + GLAD)
    bool OGLRender::init(Window *window)
    {
        __super::init(window);

        if (!glfwInit())
        {
            std::cerr << "[ERROR] Couldn't initialize GLFW" << std::endl;
            return false;
        }
        else
        {
            std::cout << "[INFO] GLFW initialized" << std::endl;
        }

        // error context
        glfwSetErrorCallback(glfw_error_callback);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);

        glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

        // adjust these values depending on the OpenGL supported by your GPU driver
        std::string glsl_version = "";
#ifdef __APPLE__
        // GLSL 150 + GL 4.1
        glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        // required on Mac OS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#elif __linux__
        // GLSL 150 + GL 4.3
        glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif _WIN32
        // GLSL 130 + GL 4.6
        glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _WIN32
        // if it's a HighDPI monitor, try to scale everything
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        float xscale, yscale;
        glfwGetMonitorContentScale(monitor, &xscale, &yscale);
        std::cout << "[INFO] Monitor scale: " << xscale << "x" << yscale << std::endl;
        if (xscale > 1 || yscale > 1)
        {
            highDPIscaleFactor = xscale;
            glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        }
#elif __APPLE__
        // to prevent 1200x800 from becoming 2400x1600
        // and some other weird resizings
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

        // const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        // glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        auto glfWindow = glfwCreateWindow(
            WINDOW_WIDTH,  // mode->width,
            WINDOW_HEIGHT, // mode->height,
            PROGRAM_NAME,
            NULL, // monitor
            NULL);
        this->window->set_native_window(glfWindow);

        if (!glfWindow)
        {
            std::cerr << "[ERROR] Couldn't create a GLFW window" << std::endl;
            return false;
        }

        glfwSetWindowPos(glfWindow, 100, 100);
        glfwSetWindowSizeLimits(
            glfWindow,
            static_cast<int>(900 * HIGH_DPI_SCALE_FACTOR),
            static_cast<int>(500 * HIGH_DPI_SCALE_FACTOR),
            GLFW_DONT_CARE,
            GLFW_DONT_CARE);

        glfwSetWindowUserPointer(glfWindow, window);
        glfwSetKeyCallback(glfWindow, on_key_callback);
        glfwSetScrollCallback(glfWindow, on_scroll_callback);
        glfwSetWindowSizeCallback(glfWindow, on_window_size_callback);
        glfwSetWindowCloseCallback(glfWindow, on_window_close_callback);
        glfwSetFramebufferSizeCallback(glfWindow, framebuffer_size_callback);
        glfwMakeContextCurrent(glfWindow);

        // OpenGL Debug message control (needs OpenGL 4.3)
        //glDebugMessageCallback(glMessageCallback, NULL);
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL, GL_TRUE);

        // VSync
        glfwSwapInterval(1);

        std::cout << "[INFO] OpenGL from GLFW "
                  << glfwGetWindowAttrib(glfWindow, GLFW_CONTEXT_VERSION_MAJOR)
                  << "."
                  << glfwGetWindowAttrib(glfWindow, GLFW_CONTEXT_VERSION_MINOR)
                  << std::endl;

        // GLAD INIT

        // load all OpenGL function pointers with glad
        // without it not all the OpenGL functions will be available,
        // such as glGetString(GL_RENDERER), and application might just segfault
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
            return false;
        }
        else
        {
            std::cout << "[INFO] GLAD initialized" << std::endl;
        }

        glEnable(GL_DEPTH_TEST);

        std::cout << "[INFO] OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "[INFO] OpenGL from glad "
                  << GLVersion.major << "." << GLVersion.minor
                  << std::endl;

        return true;
    }

    void OGLRender::pre_render()
    {
        glViewport(0, 0, window->width, window->height);
        glClearColor(0.2f, 0.2f, 0.2f, background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OGLRender::post_render()
    {
        glfwPollEvents();
        glfwSwapBuffers(static_cast<GLFWwindow *>(window->get_native_window()));
    }

    void OGLRender::end()
    {
        glfwDestroyWindow(static_cast<GLFWwindow *>(window->get_native_window()));
        glfwTerminate();
    }
}
