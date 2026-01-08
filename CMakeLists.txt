cmake_minimum_required(VERSION 3.27)
project(GameEngine)
set(ProjectDir ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DPROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/")
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# Enable big object files for MinGW (needed for sol2 templates)
if(MINGW)
    add_compile_options(-Wa,-mbig-obj)
endif()

# Fetch Lua and sol2 for scripting
include(FetchContent)

# Lua - using lua-cmake wrapper for easier building
FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/walterschell/Lua.git
    GIT_TAG v5.4.5
)
set(LUA_BUILD_COMPILER OFF CACHE BOOL "" FORCE)
set(LUA_BUILD_INTERPRETER OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(lua)

# sol2 - C++ Lua bindings (header-only)
FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol2)

add_executable(GameEngine
        engine/eventing/Event.h
        engine/eventing/EventDispatcher.h
        engine/eventing/EventListener.h
        engine/eventing/events/KeyEvent.h

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
        engine/eventing/events/CameraKeyMovementEvent.h
        engine/eventing/events/CursorEvent.h
        engine/eventing/events/MouseButtonClickEvent.h
        engine/eventing/events/MouseMovementEvent.h
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
        engine/ecs/components/CameraComponent.h
        engine/ecs/components/BoundsComponent.h

        # ECS Systems
        engine/ecs/systems/CameraSystem.h
        engine/ecs/systems/CameraInputSystem.h
        engine/ecs/systems/FrustumCullingSystem.h
        engine/ecs/systems/MeshRenderSystem.h

        engine/core/shaders/ShaderManager.h
        test_game/scenes/PartitionScene.h
        engine/core/math/Plane.h
        engine/core/math/Frustrum.h
        engine/eventing/events/DebugCameraKeyMovementEvent.h
        engine/runtime/IApplication.h
        engine/runtime/ApplicationConfig.h
        engine/runtime/ApplicationRunner.h
        engine/runtime/ApplicationRunner.cpp
        engine/platform/Window.h
        engine/platform/Window.cpp
        engine/platform/WindowConfig.h

        # Dear ImGui
        externals/imgui/imgui.cpp
        externals/imgui/imgui_draw.cpp
        externals/imgui/imgui_tables.cpp
        externals/imgui/imgui_widgets.cpp
        externals/imgui/imgui_demo.cpp
        externals/imgui/backends/imgui_impl_glfw.cpp
        externals/imgui/backends/imgui_impl_opengl3.cpp

        # Debug UI System
        engine/debug/DebugUI.h
        engine/debug/DebugUI.cpp

        # Scripting System
        engine/scripting/ScriptComponent.h
        engine/scripting/ScriptSystem.h
        engine/scripting/LuaBindings.h
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

# Dear ImGui
add_library(imgui INTERFACE)
target_include_directories(imgui INTERFACE
        "${ProjectDir}/imgui"
        "${ProjectDir}/imgui/backends")

# Link Libraries
target_link_libraries(GameEngine glfw glad glm khr stb imgui lua_static sol2)
