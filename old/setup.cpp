#include "config.h"
#include "setup.h"

std::string read_shader(std::string current_path, int vertex_type)
{
    // reads a shader from a txt file and returns it as a string
    std::string shader_path = current_path + SHADER_FOLDER;

    switch (vertex_type)
    {
    case VERTEX_SHADER:
        shader_path += VERTEX_SHADER_NAME;
        break;
    case FRAGMENT_SHADER:
        shader_path += FRAGMENT_SHADER_NAME;
        break;
    default:
        return NULL;
    }

    std::ifstream shader(shader_path);
    std::string shader_code;

    if (shader.is_open())
    {
        std::string line;
        while (std::getline(shader, line))
        {
            shader_code += line + "\n";
        }
        shader.close();
    }
    else
    {
        std::cerr << "[ERROR] Couldn't open shader file: " << shader_path << std::endl;
    }

    return shader_code;
}

static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool initializeGLFW(GLFWwindow **glfWindow)
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        std::cerr << "[ERROR] Couldn't initialize GLFW" << std::endl;
        return false;
    }
    else
    {
        std::cout << "[INFO] GLFW initialized" << std::endl;
    }

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
    *glfWindow = glfwCreateWindow(
        WINDOW_WIDTH,  // mode->width,
        WINDOW_HEIGHT, // mode->height,
        PROGRAM_NAME,
        NULL, // monitor
        NULL);
    if (!*glfWindow)
    {
        std::cerr << "[ERROR] Couldn't create a GLFW window" << std::endl;
        return false;
    }

    glfwSetWindowPos(*glfWindow, 100, 100);
    glfwSetWindowSizeLimits(
        *glfWindow,
        static_cast<int>(900 * HIGH_DPI_SCALE_FACTOR),
        static_cast<int>(500 * HIGH_DPI_SCALE_FACTOR),
        GLFW_DONT_CARE,
        GLFW_DONT_CARE);

    // watch window resizing
    glfwSetFramebufferSizeCallback(*glfWindow, framebuffer_size_callback);

    glfwMakeContextCurrent(*glfWindow);
    // VSync
    glfwSwapInterval(1);

    std::cout << "[INFO] OpenGL from GLFW "
              << glfwGetWindowAttrib(*glfWindow, GLFW_CONTEXT_VERSION_MAJOR)
              << "."
              << glfwGetWindowAttrib(*glfWindow, GLFW_CONTEXT_VERSION_MINOR)
              << std::endl;

    return true;
}

bool initializeGLAD()
{
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

    std::cout << "[INFO] OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "[INFO] OpenGL from glad "
              << GLVersion.major << "." << GLVersion.minor
              << std::endl;

    return true;
}

bool initializeDearImGui(GLFWwindow *glfWindow)
{
    if (!glfWindow)
    {
        std::cerr << "[ERROR] Couldn't create a GLFW window" << std::endl;
        return false;
    }
    std::cout << "[INFO] Dear ImGui initialization..." << std::endl;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    std::cout << "[INFO] Adding font from file TTF..." << std::endl;
    io.Fonts->AddFontFromFileTTF(
        FONT_NAME,
        24.0f * HIGH_DPI_SCALE_FACTOR,
        NULL,
        NULL);

    std::cout << "[INFO] Setting ImGui style..." << std::endl;
    setImGuiStyle(HIGH_DPI_SCALE_FACTOR);
    
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    std::cout << "[INFO] Dear ImGui style set" << std::endl;

    // setup platform/renderer bindings
    std::cout << "[INFO] Initializing ImGui_ImplGlfw..." << std::endl;
    if (!ImGui_ImplGlfw_InitForOpenGL(glfWindow, true))
    {
        std::cerr << "[ERROR] Failed to initialize ImGui_ImplGlfw" << std::endl;
        return false;
    }

    std::cout << "[INFO] Initializing ImGui_ImplOpenGL3..." << std::endl;
    if (!ImGui_ImplOpenGL3_Init())
    {
        std::cerr << "[ERROR] Failed to initialize ImGui_ImplOpenGL3" << std::endl;
        return false;
    }

    std::cout << "[INFO] Dear ImGui initialized" << std::endl;

    return true;
}

int initAll(GLFWwindow **glfWindow)
{
    if (!initializeGLFW(glfWindow))
    {
        std::cerr << "[ERROR] GLFW initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!initializeGLAD())
    {
        std::cerr << "[ERROR] glad initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!initializeDearImGui(*glfWindow))
    {
        std::cerr << "[ERROR] Dear ImGui initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

void teardown(GLFWwindow *glfWindow, unsigned int shaderProgram, unsigned int VBO, unsigned int VAO)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    if (glfWindow != NULL)
    {
        glfwDestroyWindow(glfWindow);
    }
    glfwTerminate();
}