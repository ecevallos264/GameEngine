#ifndef GAMEENGINE_TESTAPPLICATION_H
#define GAMEENGINE_TESTAPPLICATION_H

#include "../engine/runtime/IApplication.h"
#include "../engine/runtime/SystemManager.h"
#include "../engine/runtime/systems/InputSystem.h"
#include "../engine/runtime/systems/PhysicsSystem.h"
#include "../engine/runtime/systems/RenderSystem.h"
#include "../engine/runtime/systems/SceneSystem.h"
#include "../engine/ecs/ECS.h"
#include "../engine/core/shaders/shader-compiler.h"
#include "../engine/core/shaders/ShaderManager.h"
#include "../engine/core/shaders/ShaderInfo.h"
#include "../engine/core/state/game_state.h"
#include "../engine/rendering/SceneController.h"
#include "../engine/eventing/EventDispatcher.h"
#include "../engine/eventing/events/MouseMovementEvent.h"
#include "../engine/ecs/events/CameraFocusEvent.h"
#include "../engine/io/IOSystem.h"
#include "../engine/debug/DebugUI.h"
#include "scenes/TestScene.h"
#include <GLFW/glfw3.h>
#include <imgui.h>

class TestApplication : public IApplication {
public:
    void configure(ApplicationConfig& config) override {
        config.windowWidth = 1600;
        config.windowHeight = 1200;
        config.windowTitle = "Game Engine";
        config.vsync = false;
    }

    void initialize(Window& window) override {
        this->window = &window;
        context.window = &window;

        // Initialize IOSystem singleton first (sets up input callbacks)
        IO::IOSystem::getInstance().initialize(context);

        // Initialize Debug UI
        Debug::DebugUI::getInstance().initialize(
            static_cast<GLFWwindow*>(window.getNativeHandle()));

        // Setup camera mouse movement callback
        setupCameraMouseCallback(window);

        // Setup ESC key callback for camera focus toggle
        setupKeyCallback(window);

        ShaderInfo shaderInfo;
        shaderInfo.VertexShaderPath = "C:\\Users\\eceva\\CLionProjects\\GameEngine\\test_game\\shaders\\shader.vert";
        shaderInfo.FragmentShaderPath = "C:\\Users\\eceva\\CLionProjects\\GameEngine\\test_game\\shaders\\shader.frag";

        ShaderManager::getInstance().setShader("shader1", new Shader(
            shaderInfo.VertexShaderPath.c_str(),
            shaderInfo.FragmentShaderPath.c_str()));

        auto scene = std::make_shared<TestScene>(
            new Shader(
                shaderInfo.VertexShaderPath.c_str(),
                shaderInfo.FragmentShaderPath.c_str()));

        SceneController::getInstance().addScene("testScene", scene);
        SceneController::getInstance().swapScene("testScene");

        // Create ECS camera entity
        cameraEntity = registry.create();
        registry.emplace<ECS::TransformComponent>(cameraEntity, glm::vec3(0.0f, 0.0f, 3.0f));

        auto& cam = registry.emplace<ECS::CameraComponent>(cameraEntity);
        cam.tag = ECS::CameraTag::Main;
        cam.mode = ECS::CameraMode::FPS;
        cam.moveSpeed = 5.0f;
        cam.fov = 45.0f;

        // Make camera controllable
        registry.emplace<ECS::CameraControllerComponent>(cameraEntity);

        // Add ECS camera systems first (order matters!)
        cameraSystem = &systemManager.addSystem<ECS::CameraSystem>(registry);
        systemManager.addSystem<ECS::CameraInputSystem>(registry, *cameraSystem);
        systemManager.addSystem<ECS::FrustumCullingSystem>(registry, *cameraSystem);

        // Set camera system in context for other systems to use
        context.cameraSystem = cameraSystem;

        // Add other systems
        systemManager.addSystem<InputSystem>();
        systemManager.addSystem<PhysicsSystem>();
        systemManager.addSystem<SceneSystem>();
        systemManager.addSystem<RenderSystem>();

        systemManager.initializeAll(context);
    }

    void update(float deltaTime) override {
        GameState::getInstance().deltaTime = deltaTime;
        GameState::getInstance().lastFrame += deltaTime;

        context.deltaTime = deltaTime;
        systemManager.updateAll(context);

        // Clear per-frame input state at end of frame
        IO::IOSystem::getInstance().endFrame();
    }

    void render() override {
        systemManager.renderAll(context);

        // Render Debug UI
        Debug::DebugUI::getInstance().beginFrame();
        Debug::DebugUI::getInstance().endFrame();
    }

    void shutdown() override {
        Debug::DebugUI::getInstance().shutdown();
        systemManager.shutdownAll(context);
        IO::IOSystem::getInstance().shutdown(context);
    }

private:
    Window* window = nullptr;
    SystemManager systemManager;
    SystemContext context;
    ECS::Registry registry;
    ECS::Entity cameraEntity;
    ECS::CameraSystem* cameraSystem = nullptr;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
    bool cameraFocused = true;

    void setupCameraMouseCallback(Window& window) {
        // Set mouse callback that updates IOSystem and dispatches mouse events
        window.setMouseMoveCallback([this](double x, double y) {
            // Update IOSystem mouse state
            IO::IOSystem::getInstance().getInputState().mouse.setPosition(x, y);

            // Calculate delta for mouse movement event
            if (firstMouse) {
                lastMouseX = x;
                lastMouseY = y;
                firstMouse = false;
                return;
            }

            double deltaX = x - lastMouseX;
            double deltaY = lastMouseY - y;  // Inverted for natural feel

            lastMouseX = x;
            lastMouseY = y;

            // Dispatch mouse movement event for CameraInputSystem
            EventDispatcher::getInstance().dispatch(
                MouseMovementEvent(
                    deltaX,
                    deltaY,
                    MouseCursorState::IN_WINDOW,
                    GameState::getInstance().deltaTime));
        });
    }

    void setupKeyCallback(Window& window) {
        window.setKeyCallback([this](int key, int scancode, int action, int mods) {
            (void)scancode;
            (void)mods;

            // ESC toggles camera focus
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                cameraFocused = !cameraFocused;

                // Dispatch focus event
                EventDispatcher::getInstance().dispatch(ECS::CameraFocusEvent(cameraFocused));

                // Toggle cursor mode
                if (cameraFocused) {
                    this->window->setCursorMode(CursorMode::Disabled);
                    firstMouse = true;  // Reset to avoid camera jump on refocus
                } else {
                    this->window->setCursorMode(CursorMode::Normal);
                }
            }

            // F3 toggles debug UI
            if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
                Debug::DebugUI::getInstance().toggle();
            }
        });
    }
};

#endif //GAMEENGINE_TESTAPPLICATION_H
