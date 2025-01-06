#include "ui.h"

#include "config.h"

#include <iostream>

namespace renderer
{
    bool UIContext::init(Window *window)
    {
        __super::init(window);

        // Setup Dear ImGui context
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

        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        std::cout << "[INFO] Dear ImGui style set" << std::endl;

        // setup platform/renderer bindings
        std::cout << "[INFO] Initializing ImGui_ImplGlfw..." << std::endl;
        if (!ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)this->window->get_native_window(), true))
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

    void UIContext::pre_render()
    {
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

    void UIContext::post_render()
    {
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO &io = ImGui::GetIO();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void UIContext::end()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        // glfwTerminate();
    }
}