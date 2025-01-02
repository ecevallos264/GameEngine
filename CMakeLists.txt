cmake_minimum_required(VERSION 3.27)
project(GameEngine)
set(ProjectDir ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(GameEngine main.cpp
        glad.c
        stb.cpp
        shader-compiler.h
        camera.h
        Singleton.h
        game.h
        game.cpp
        shape.h
        entities/line.h
        entities/xPlane.h
        my_math.h
        entities/sphere.h
        entities/particle.h
        Entity_Handler.h
        entities/cube.h
        entities/hex-prism.h
        entities/triangular-prism.h
        entities/pyramid.h
        entities/vector.h
        settings.h
        gui.h
)

target_compile_definitions(GameEngine PRIVATE GLFW_INCLUDE_NONE)

# GLFW
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${ProjectDir}/GLFW/lib/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${ProjectDir}/GLFW/include")

# ImGui
# Add ImGui library
add_library(imgui
        ${ProjectDir}/imgui/imgui.cpp
        ${ProjectDir}/imgui/imgui_draw.cpp
        ${ProjectDir}/imgui/imgui_tables.cpp
        ${ProjectDir}/imgui/imgui_widgets.cpp
        ${ProjectDir}/imgui/imgui_impl_glfw.cpp
        ${ProjectDir}/imgui/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PRIVATE
        ${ProjectDir}/imgui
        ${ProjectDir}/imgui/backends
        ${ProjectDir}
)

target_link_libraries(imgui glfw)

# GLAD
add_library(glad INTERFACE
        )
target_include_directories(glad INTERFACE "${ProjectDir}/GLAD/include")

# GLM (header-only, no lib needed)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${ProjectDir}/GLM/include")

# KHR (assumed to be header-only, used by GLAD)
add_library(khr INTERFACE)
target_include_directories(khr INTERFACE "${ProjectDir}/KHR/include")

# STB (assumed to be header-only)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${ProjectDir}/STB/include")

# Link Libraries
target_link_libraries(GameEngine glfw glad glm khr stb imgui)