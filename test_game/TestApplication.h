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
#include "../engine/debug/SceneViewport.h"
#include "../engine/debug/EditorLayout.h"
#include "../engine/ecs/systems/BVHSystem.h"
#include "../engine/rendering/Scene.h"
#include "../engine/scripting/ScriptSystem.h"
#include "scenes/TestScene.h"
#include "scenes/ScriptedScene.h"
#include <glad/glad.h>
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

        // Initialize scene viewport for rendering to ImGui window
        sceneViewport.initialize(window.getWidth(), window.getHeight());

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
        // auto scene = std::make_shared<ScriptedScene>(
        //   new Shader(
        //       shaderInfo.VertexShaderPath.c_str(),
        //       shaderInfo.FragmentShaderPath.c_str()),
        //   "C:\\Users\\eceva\\CLionProjects\\GameEngine\\test_game\\scripts");


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

        // Add BVH system for spatial acceleration
        bvhSystem = &systemManager.addSystem<ECS::BVHSystem>(*cameraSystem);

        // Set camera system in context for other systems to use
        context.cameraSystem = cameraSystem;

        // Add scripting system (must be after scene is set up)
        systemManager.addSystem<Scripting::ScriptSystem>();

        // Add other systems
        systemManager.addSystem<InputSystem>();
        systemManager.addSystem<PhysicsSystem>();
        systemManager.addSystem<SceneSystem>();
        systemManager.addSystem<RenderSystem>();

        systemManager.initializeAll(context);

        // Initialize BVH debug renderer
        bvhDebugRenderer.initialize();
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
        // Render scene to framebuffer
        sceneViewport.beginSceneRender();
        systemManager.renderAll(context);

        // Render BVH visualization if enabled
        if (showBVH && bvhSystem && cameraSystem) {
            glm::mat4 view = cameraSystem->getViewMatrix();
            glm::mat4 projection = cameraSystem->getProjectionMatrix();
            bvhDebugRenderer.render(bvhSystem->getBVHTree(), view, projection, bvhMaxDepth, showBVHLeafOnly);
        }

        sceneViewport.endSceneRender();

        // Restore viewport to window size for ImGui
        glViewport(0, 0, window->getWidth(), window->getHeight());

        // Render Editor UI
        Debug::DebugUI::getInstance().beginFrame();

        // Begin editor layout
        editorLayout.beginFrame(window->getWidth(), window->getHeight());

        // Left panel - Hierarchy
        editorLayout.renderLeftPanel("Hierarchy", [this]() {
            ImGui::Text("Scene Objects");
            ImGui::Separator();
            if (ImGui::TreeNode("Main Camera")) {
                ImGui::Text("Entity ID: %llu", cameraEntity.id);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Test Scene")) {
                ImGui::Text("Active Scene");
                ImGui::TreePop();
            }
        });

        // Right panel - Inspector
        editorLayout.renderRightPanel("Inspector", [this]() {
            ImGui::Text("Properties");
            ImGui::Separator();

            if (cameraSystem && cameraSystem->getActiveCamera().isValid()) {
                auto* cam = registry.get<ECS::CameraComponent>(cameraEntity);
                auto* transform = registry.get<ECS::TransformComponent>(cameraEntity);

                if (transform) {
                    ImGui::Text("Transform");
                    ImGui::DragFloat3("Position", &transform->position.x, 0.1f);
                }

                if (cam) {
                    ImGui::Separator();
                    ImGui::Text("Camera");
                    ImGui::DragFloat("FOV", &cam->fov, 1.0f, 1.0f, 120.0f);
                    ImGui::DragFloat("Move Speed", &cam->moveSpeed, 0.1f, 0.1f, 50.0f);
                    ImGui::DragFloat("Near Plane", &cam->nearPlane, 0.01f, 0.01f, 10.0f);
                    ImGui::DragFloat("Far Plane", &cam->farPlane, 1.0f, 10.0f, 10000.0f);
                }
            }

            ImGui::Separator();
            ImGui::Text("Camera: %s", cameraFocused ? "Focused (ESC to release)" : "Released (ESC to focus)");

            // BVH Settings
            ImGui::Separator();
            if (ImGui::CollapsingHeader("BVH Spatial Tree", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (bvhSystem) {
                    // Stats
                    ImGui::Text("Visible: %d / %d", bvhSystem->getVisibleCount(), bvhSystem->getTotalCount());
                    ImGui::Text("Nodes: %d", bvhSystem->getNodeCount());
                    ImGui::Text("Tree Depth: %d", bvhSystem->getTreeDepth());

                    ImGui::Separator();

                    // Controls
                    bool bvhEnabled = bvhSystem->isEnabled();
                    if (ImGui::Checkbox("Enable BVH Culling", &bvhEnabled)) {
                        bvhSystem->setEnabled(bvhEnabled);
                    }

                    ImGui::Checkbox("Show BVH Wireframe", &showBVH);

                    if (showBVH) {
                        ImGui::Checkbox("Leaf Nodes Only", &showBVHLeafOnly);
                        ImGui::SliderInt("Max Depth", &bvhMaxDepth, -1, 10);
                        if (bvhMaxDepth == -1) {
                            ImGui::SameLine();
                            ImGui::TextDisabled("(All)");
                        }
                    }

                    if (ImGui::Button("Rebuild BVH")) {
                        bvhSystem->markDirty();
                    }
                }
            }

            // Frustum Culling Settings
            if (ImGui::CollapsingHeader("Frustum Culling", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Stats
                ImGui::Text("Rendered: %d", Scene::renderedCount);
                ImGui::Text("Culled: %d", Scene::culledCount);
                int total = Scene::renderedCount + Scene::culledCount;
                if (total > 0) {
                    float cullPercent = (Scene::culledCount / static_cast<float>(total)) * 100.0f;
                    ImGui::Text("Cull Rate: %.1f%%", cullPercent);
                }

                ImGui::Separator();

                // Controls
                ImGui::Checkbox("Enable Frustum Culling", &Scene::frustumCullingEnabled);
                ImGui::Checkbox("Show Culled Objects (Wireframe)", &Scene::showCulledObjects);

                if (Scene::showCulledObjects) {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Culled objects shown as wireframe");
                }
            }
        });

        // Bottom panel - Console
        editorLayout.renderBottomPanel("Console", []() {
            ImGui::Text("Output Log");
            ImGui::Separator();
            ImGui::TextWrapped("Game Engine initialized successfully.");
            ImGui::TextWrapped("Scene loaded: TestScene");
        });

        // Center viewport - Scene
        editorLayout.renderCenterViewport("Scene", [this]() {
            sceneViewport.renderContent(cameraSystem);
        });

        editorLayout.endFrame();

        Debug::DebugUI::getInstance().endFrame();
    }

    void shutdown() override {
        bvhDebugRenderer.shutdown();
        sceneViewport.shutdown();
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
    ECS::BVHSystem* bvhSystem = nullptr;
    ECS::BVHDebugRenderer bvhDebugRenderer;
    Debug::SceneViewport sceneViewport;
    Debug::EditorLayout editorLayout;

    // BVH visualization settings
    bool showBVH = false;
    bool showBVHLeafOnly = false;
    int bvhMaxDepth = -1;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
    bool cameraFocused = false;  // Start unfocused for editor mode

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

            // Forward to IOSystem for input state tracking
            IO::IOSystem::getInstance().getInputState().keyboard.setKeyState(key, action);

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
