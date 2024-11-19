cmake_minimum_required(VERSION 3.27)
project(SoftwareEngineeringProject)
set(ProjectDir ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(SoftwareEngineeringProject main.cpp
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
)

target_compile_definitions(SoftwareEngineeringProject PRIVATE GLFW_INCLUDE_NONE)

# GLFW
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${ProjectDir}/GLFW/lib/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${ProjectDir}/GLFW/include")

# GLAD
add_library(glad INTERFACE
        entities/cube.h
        entities/hex-prism.h
        entities/triangular-prism.h
        entities/pyramid.h)
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
target_link_libraries(SoftwareEngineeringProject glfw glad glm khr stb)