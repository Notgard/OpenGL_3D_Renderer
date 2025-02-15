cmake_minimum_required(VERSION 3.21)

# Project definition
project("glfw-imgui"
    DESCRIPTION "ImGUI App"
    LANGUAGES C CXX
)

message("Project name: " ${CMAKE_PROJECT_NAME})

# Set C++ standard and export compile commands
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set postfix for debug configuration
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Filename postfix for libraries under DEBUG configuration")

#add_compile_options(
#"-Wno-unused-local-typedefs"
#)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs" )

# Define source and header files
set(sources
    src/app.cpp 
    src/buffer.cpp 
    src/camera.cpp
    src/grid.cpp
    src/input_handler.cpp 
    src/functions.cpp
    src/main.cpp
    src/menu_bar.cpp
    src/mesh.cpp
    src/object.cpp
    src/ogl_window.cpp 
    src/ogl.cpp 
    src/scene_view.cpp 
    src/shader.cpp 
    src/ui.cpp 
)

set(headers
    includes/app.h 
    includes/buffer.h
    includes/camera.h
    includes/config.h
    includes/grid.h 
    includes/functions.h 
    includes/imgui_stdlib.h 
    includes/imgui-style.h 
    includes/input_handler.h 
    includes/menu_bar.h
    includes/mesh.h
    includes/object.h
    includes/ogl_window.h 
    includes/ogl.h 
    includes/render.h 
    includes/scene_view.h 
    includes/shader.h 
    includes/ui.h 
    includes/vertex.h
    includes/window.h
)

set(resource_files
    ${CMAKE_SOURCE_DIR}/includes/JetBrainsMono-ExtraLight.ttf
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# If UNIX, find necessary dependencies for non-Apple systems
if (UNIX)
    if (NOT APPLE)
        find_package(Threads REQUIRED)
        find_package(X11 REQUIRED)
    endif()
endif()

# Find dependencies: ImGui, GLFW, and GLAD
find_package(imgui REQUIRED)
message("ImGui version name: " ${imgui_VERSION})
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

# Define the executable
add_executable(${CMAKE_PROJECT_NAME} ${sources} ${headers}
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_glfw.h
    bindings/imgui_impl_opengl3.cpp
    bindings/imgui_impl_opengl3.h
    bindings/imgui_impl_opengl3_loader.h
    bindings/stb_image.h
    bindings/stb_image_write.h
    bindings/imfilebrowser.h
)

# Specify include directories for the compiler
target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/bindings
)

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE imgui::imgui glad::glad glfw OpenGL::GL glm::glm assimp::assimp
)

# Add source files to the target
target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE ${sources}
)

# Platform-specific linking
if (UNIX)
    if (APPLE)
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                "-framework Cocoa"
                "-framework IOKit"
                "-framework CoreVideo"
        )
    else()
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                ${CMAKE_THREAD_LIBS_INIT}
                ${X11_LIBRARIES}
                ${CMAKE_DL_LIBS}
        )
    endif()
endif()

# Set target properties for debug builds
set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
)

# Install configuration
include(GNUInstallDirs)

install(TARGETS ${CMAKE_PROJECT_NAME}
    COMPONENT ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME}
)

install(FILES
    ${resource_files}
    DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME}
    COMPONENT ${CMAKE_PROJECT_NAME}
)
