cmake_minimum_required(VERSION 3.27)
project(GameEngine)
set(ProjectDir ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_executable(GameEngine
        engine/camera/Camera.cpp
        engine/camera/Camera.h
        engine/camera/CameraHandler.cpp
        engine/camera/CameraHandler.h
        engine/entitities/Shape.h
        engine/core/eventing/Event.h
        engine/core/eventing/EventDispatcher.h
        engine/core/eventing/EventListener.h
        engine/core/eventing/events/KeyEvent.h
        engine/input/InputHandler.cpp
        engine/input/InputHandler.h
        engine/input/MouseHandler.h
        engine/core/observability/CalcTime.h
        engine/core/observability/FPSCounter.h
        engine/core/patterns/Singleton.h
        engine/core/settings/settings.h
        engine/core/shaders/ShaderInfo.h
        engine/core/shaders/shader-compiler.h
        engine/core/state/MouseState.h
        engine/core/state/game_state.h
        engine/engine.h
        engine/core/eventing/MouseEvents.h
        engine/core/eventing/events/CameraKeyMovementEvent.h
        engine/core/eventing/events/CursorEvent.h
        engine/core/eventing/events/MouseButtonClickEvent.h
        engine/core/eventing/events/MouseMovementEvent.h
        engine/game.cpp
        engine/game.h
        engine/glad.c
        engine/stb.cpp

        test_game/entities/Player.h
        test_game/main.cpp
        test_game/mouse.h
        engine/core/types/MouseState.h
        engine/rendering/Scene.h
        engine/rendering/EntityController.h
        engine/rendering/Entity.h
        engine/rendering/Renderer.h
        engine/rendering/Scene.cpp
        engine/rendering/EntityController.cpp
        engine/rendering/Entity.cpp
        engine/rendering/SceneController.h
        test_game/scenes/TestScene.h
        engine/entitities/Cube.h
        engine/entitities/Vertex.h
        engine/physics/Simplex.h
        engine/physics/CollisionHandler.h
        engine/physics/GJK/GJK.h
        engine/physics/GJK/GJK.cpp
        test_game/entities/MyCube.h
        engine/physics/CollisionEvent.h
        engine/entitities/Point.h
        engine/rendering/BoundingRegion.h
        engine/entitities/Rectangle.h
        engine/rendering/RenderableEntity.h
        test_game/entities/HexagonalPrism.h
        engine/rendering/Oct-Tree.h

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