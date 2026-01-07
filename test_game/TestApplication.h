#ifndef GAMEENGINE_TESTAPPLICATION_H
#define GAMEENGINE_TESTAPPLICATION_H

#include "../engine/runtime/IApplication.h"
#include "../engine/runtime/SystemManager.h"
#include "../engine/runtime/systems/InputSystem.h"
#include "../engine/runtime/systems/PhysicsSystem.h"
#include "../engine/runtime/systems/RenderSystem.h"
#include "../engine/runtime/systems/SceneSystem.h"
#include "../engine/camera/Camera.h"
#include "../engine/camera/CameraHandler.h"
#include "../engine/core/shaders/shader-compiler.h"
#include "../engine/core/shaders/ShaderManager.h"
#include "../engine/core/shaders/ShaderInfo.h"
#include "../engine/core/state/game_state.h"
#include "../engine/rendering/SceneController.h"
#include "../engine/input/MouseHandler.h"
#include "../engine/core/eventing/EventDispatcher.h"
#include "../engine/core/eventing/events/MouseMovementEvent.h"
#include "../engine/io/IOSystem.h"
#include "scenes/TestScene.h"

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

        // Setup camera mouse movement callback
        setupCameraMouseCallback(window);

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

        CameraHandler::getInstance().setCamera(new Camera(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)));
        CameraHandler::getInstance().getCamera()->setSpeed(5.0f);

        // Add other systems (not IOSystem - it's a singleton initialized above)
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
    }

    void shutdown() override {
        systemManager.shutdownAll(context);
        IO::IOSystem::getInstance().shutdown(context);
    }

private:
    Window* window = nullptr;
    SystemManager systemManager;
    SystemContext context;

    void setupCameraMouseCallback(Window& window) {
        // Set mouse callback that updates both IOSystem and camera
        window.setMouseMoveCallback([](double x, double y) {
            // Update IOSystem mouse state
            IO::IOSystem::getInstance().getInputState().mouse.setPosition(x, y);

            // Handle camera movement
            Camera* camera = CameraHandler::getInstance().getCamera();
            if (camera) {
                EventDispatcher::getInstance().dispatch(
                    MouseMovementEvent(
                        x - camera->getXPosition(),
                        camera->getYPosition() - y,
                        MouseHandler::getInstance().getMouseCursorState(),
                        GameState::getInstance().deltaTime));

                camera->setXPosition(x);
                camera->setYPosition(y);
            }
        });
    }
};

#endif //GAMEENGINE_TESTAPPLICATION_H
