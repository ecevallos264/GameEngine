cmake_minimum_required(VERSION 3.27)
project(GameEngine)
set(ProjectDir ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(GameEngine test_game/main.cpp
        engine/glad.c
        engine/stb.cpp
        engine/utils/shaders/shader-compiler.h
        engine/camera/camera_old.h
        engine/utils/patterns/Singleton.h
        engine/game/game.h
        engine/game/game.cpp
        engine/entitities/shape.h
        engine/utils/my_math.h

        engine/entitities/EntityHandler.h
        engine/utils/input/InputHandler.h
        engine/utils/input/InputHandler.cpp
        engine/utils/settings/settings.h
        engine/utils/state/game_state.h


        engine/eventing/Event.h
        engine/eventing/EventDispatcher.h
        engine/eventing/EventListener.h
        engine/eventing/events/KeyEvent.h
        engine/engine.h
        engine/utils/shaders/ShaderInfo.h
        engine/utils/state/MouseState.h
        test_game/entities/Player.h
        test_game/mouse.h
        engine/eventing/MouseEvents.h
        engine/entitities/Entity.h
        engine/eventing/events/CursorEvent.h
        engine/camera/Camera.h
        engine/camera/CameraHandler.h
        engine/camera/CameraHandler.cpp
        engine/eventing/events/CameraMovementEvent.h
        engine/camera/CameraMovements.h
)

target_compile_definitions(GameEngine PRIVATE GLFW_INCLUDE_NONE)

# GLFW
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${ProjectDir}/GLFW/lib/libglfw3.a"
        INTERFACE_INCLUDE_DIRECTORIES "${ProjectDir}/GLFW/include")

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
target_link_libraries(GameEngine glfw glad glm khr stb)