//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_TESTSCENE_H
#define GAMEENGINE_TESTSCENE_H

#include "../../engine/rendering/Scene.h"
#include "../../engine/core/shaders/shader-compiler.h"
#include "../../engine/core/patterns/Singleton.h"
#include "../../engine/core/shaders/ShaderManager.h"
#include "../../engine/input/InputHandler.h"
#include "../../engine/input/MouseHandler.h"
#include "../../engine/core/state/game_state.h"
#include "../../engine/core/eventing/EventDispatcher.h"
#include "../../engine/core/eventing/events/CameraKeyMovementEvent.h"

#include "../../engine/ecs/ECS.h"

// Helper to create cube mesh data
namespace CubeMesh {
    inline std::vector<Vertex> createVertices(const glm::vec3& color, float alpha = 0.5f) {
        return {
            {glm::vec3(-0.5f, -0.5f,  0.5f), color, alpha},
            {glm::vec3( 0.5f, -0.5f,  0.5f), color, alpha},
            {glm::vec3( 0.5f,  0.5f,  0.5f), color, alpha},
            {glm::vec3(-0.5f,  0.5f,  0.5f), color, alpha},
            {glm::vec3(-0.5f, -0.5f, -0.5f), color, alpha},
            {glm::vec3( 0.5f, -0.5f, -0.5f), color, alpha},
            {glm::vec3( 0.5f,  0.5f, -0.5f), color, alpha},
            {glm::vec3(-0.5f,  0.5f, -0.5f), color, alpha}
        };
    }

    inline std::vector<unsigned int> getIndices() {
        return {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 3, 7, 7, 4, 0,
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0
        };
    }
}

class TestScene : public Scene {
private:
    ECS::Entity mainCube;

public:
    TestScene(Shader* shader) : Scene() {
        ShaderManager::getInstance().setShader("shader1", shader);
        setup();

        // Create main cube using ECS
        mainCube = createEntity();
        registry.emplace<ECS::TransformComponent>(mainCube, glm::vec3(0.0f, 0.0f, 0.0f));
        registry.emplace<ECS::MeshComponent>(mainCube,
            CubeMesh::createVertices(glm::vec3(1.0f, 0.0f, 0.0f)),
            CubeMesh::getIndices());
        registry.emplace<ECS::RenderComponent>(mainCube, shader, glm::vec3(1.0f, 0.0f, 0.0f));

        std::srand(static_cast<unsigned>(std::time(0)));
    }

    void setup() override {
    }

    int handleInput(GLFWwindow* window) override {
        if (InputHandler::isKeyActive(GLFW_KEY_ESCAPE)) {
            MouseHandler::getInstance().changeMouseMode(MouseCursorState::OUT_OF_WINDOW);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GameState::getInstance().CURSOR_FOCUS_STATUS = true;
        }

        // Spawn new cube with Enter key
        if (InputHandler::isKeyActive(GLFW_KEY_ENTER)) {
            ECS::Entity newCube = createEntity();
            registry.emplace<ECS::TransformComponent>(newCube, glm::vec3(0.0f, 0.0f, 0.0f));
            registry.emplace<ECS::MeshComponent>(newCube,
                CubeMesh::createVertices(glm::vec3(1.0f, 0.0f, 0.0f)),
                CubeMesh::getIndices());
            registry.emplace<ECS::RenderComponent>(newCube,
                ShaderManager::getInstance().getShader("shader1"),
                glm::vec3(1.0f, 0.0f, 0.0f));
        }

        // Move the main cube with arrow keys
        float deltaTime = GameState::getInstance().deltaTime;
        auto* transform = registry.get<ECS::TransformComponent>(mainCube);
        if (transform) {
            if (InputHandler::isKeyActive(GLFW_KEY_UP)) {
                transform->position.y += deltaTime * 0.5f;
            }
            if (InputHandler::isKeyActive(GLFW_KEY_DOWN)) {
                transform->position.y -= deltaTime * 0.5f;
            }
            if (InputHandler::isKeyActive(GLFW_KEY_LEFT)) {
                transform->position.x -= deltaTime * 0.5f;
            }
            if (InputHandler::isKeyActive(GLFW_KEY_RIGHT)) {
                transform->position.x += deltaTime * 0.5f;
            }
        }

        bool dirty = false;
        if (InputHandler::isKeyActive(GLFW_KEY_W)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::FORWARD, deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_S)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::BACKWARD, deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_A)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::LEFT, deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_D)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::RIGHT, deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_SPACE)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::UP, deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_LEFT_SHIFT)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::DOWN, deltaTime));
            dirty = true;
        }
        return dirty;
    }
};

#endif //GAMEENGINE_TESTSCENE_H
