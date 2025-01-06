#include "setup.h"
#include "shader.h"

#include "config.h"

unsigned int shaderProgram, VBO, VAO;

void preRenderImGui()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create the docking environment
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                   ImGuiWindowFlags_NoBackground;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}

void composeDearImGuiFrame(GLFWwindow *glfWindow)
{
    ImGui::Begin("Scene");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
/*     mSize = { viewportPanelSize.x, viewportPanelSize.y };

    mCamera->set_aspect(mSize.x / mSize.y);
    mCamera->update(mShader.get()); */

    // add rendered texture to ImGUI scene window
    uint64_t textureID = mFrameBuffer->get_texture();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
}

int main(int argc, char *argv[])
{
    // initialize variables
    std::string fontName = FONT_NAME;

    std::filesystem::path currentPath = ".";
    std::filesystem::path basePath = ".";

    GLFWwindow *glfWindow = NULL;
    int counter = 0;

    unsigned int shaderProgram, VBO, VAO, FBO;

    // start timer
    std::cout << "["
              << currentTime(std::chrono::system_clock::now())
              << "] "
              << "Start\n- - -\n\n";

    // setting paths to resources
    currentPath = std::filesystem::current_path();
    std::cout << "[DEBUG] Current working directory: " << currentPath << std::endl;
    // basePath = std::filesystem::path(argv[0]).parent_path();
    basePath = std::filesystem::path(argv[0]).remove_filename();
#ifndef _WIN32 // on Windows argv[0] is absolute path
    basePath = currentPath / basePath;
#endif
    // std::cout << "[DEBUG] Executable name/path: " << argv[0] << std::endl
    //           << "parent path: " << basePath << std::endl << std::endl;
    fontName = (basePath / fontName).string();

    // initialize GLFW, GLAD, Dear ImGui
    initAll(&glfWindow);

    // read shader files
    std::string vertex_string = read_shader(currentPath.string(), VERTEX_SHADER);
    const char *vertexShaderSource = vertex_string.c_str();

    std::string fragment_string = read_shader(currentPath.string(), FRAGMENT_SHADER);
    const char *fragmentShaderSource = fragment_string.c_str();

    // build and compile our shader program
    buildShaderProgram(shaderProgram, VBO, VAO, FBO, vertexShaderSource, fragmentShaderSource);

    // rendering loop
    while (!glfwWindowShouldClose(glfWindow))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        preRenderImGui();

        glUseProgram(shaderProgram);

        // Dear ImGui frame
        composeDearImGuiFrame(glfWindow);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    teardown(glfWindow, shaderProgram, VBO, VAO);

    std::cout << "\n- - -\n"
              << "["
              << currentTime(std::chrono::system_clock::now())
              << "] "
              << "Quit\n";

    return 0;
}
