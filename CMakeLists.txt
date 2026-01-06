cmake_minimum_required(VERSION 3.27)
project(GameEngine)
set(ProjectDir ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/")
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

add_executable(GameEngine
        engine/camera/Camera.cpp
        engine/camera/Camera.h
        engine/camera/CameraHandler.cpp
        engine/camera/CameraHandler.h
        engine/core/eventing/Event.h
        engine/core/eventing/EventDispatcher.h
        engine/core/eventing/EventListener.h
        engine/core/eventing/events/KeyEvent.h
        engine/input/InputHandler.h
        engine/input/MouseHandler.h

        # IO System
        engine/io/InputState.h
        engine/io/IOSystem.h
        engine/core/observability/CalcTime.h
        engine/core/observability/FPSCounter.h
        engine/core/patterns/Singleton.h
        engine/core/settings/settings.h
        engine/core/shaders/ShaderInfo.h
        engine/core/shaders/shader-compiler.h
        engine/core/state/game_state.h
        engine/core/eventing/events/CameraKeyMovementEvent.h
        engine/core/eventing/events/CursorEvent.h
        engine/core/eventing/events/MouseButtonClickEvent.h
        engine/core/eventing/events/MouseMovementEvent.h
        engine/glad.c
        engine/stb.cpp
        engine/runtime/ISystem.h
        engine/runtime/SystemContext.h
        engine/runtime/SystemManager.h
        engine/runtime/SystemManager.cpp
        engine/runtime/systems/InputSystem.h
        engine/runtime/systems/PhysicsSystem.h
        engine/runtime/systems/RenderSystem.h
        engine/runtime/systems/SceneSystem.h

        test_game/main.cpp
        test_game/TestApplication.h
        engine/core/types/MouseState.h
        engine/rendering/Scene.h
        engine/rendering/Renderer.h
        engine/rendering/Scene.cpp
        engine/rendering/SceneController.h
        test_game/scenes/TestScene.h
        engine/entities/Vertex.h

        # ECS Core
        engine/ecs/Entity.h
        engine/ecs/Component.h
        engine/ecs/ComponentPool.h
        engine/ecs/Registry.h
        engine/ecs/ECS.h

        # ECS Components
        engine/ecs/components/TransformComponent.h
        engine/ecs/components/MeshComponent.h
        engine/ecs/components/RenderComponent.h

        # ECS Systems
        engine/ecs/systems/MeshRenderSystem.h

        engine/core/shaders/ShaderManager.h
        test_game/scenes/PartitionScene.h
        engine/core/math/Plane.h
        engine/core/math/Frustrum.h
        engine/core/eventing/events/DebugCameraKeyMovementEvent.h
        engine/runtime/IApplication.h
        engine/runtime/ApplicationConfig.h
        engine/runtime/ApplicationRunner.h
        engine/runtime/ApplicationRunner.cpp
        engine/platform/Window.h
        engine/platform/Window.cpp
        engine/platform/WindowConfig.h

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
