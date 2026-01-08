//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_TESTSCENE_H
#define GAMEENGINE_TESTSCENE_H

#include <ctime>
#include "../../engine/rendering/Scene.h"
#include "../../engine/core/shaders/shader-compiler.h"
#include "../../engine/core/patterns/Singleton.h"
#include "../../engine/core/shaders/ShaderManager.h"
#include "../../engine/core/state/game_state.h"
#include "../../engine/eventing/EventDispatcher.h"
#include "../../engine/eventing/events/CameraKeyMovementEvent.h"
#include "../../engine/platform/Window.h"
#include "../../engine/io/InputState.h"
#include "../../engine/io/IOSystem.h"

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

    // Helper to create a cube with all required components
    ECS::Entity createCube(Shader* shader, const glm::vec3& position, const glm::vec3& color) {
        ECS::Entity cube = createEntity();
        registry.emplace<ECS::TransformComponent>(cube, position);
        registry.emplace<ECS::MeshComponent>(cube,
            CubeMesh::createVertices(color),
            CubeMesh::getIndices());
        registry.emplace<ECS::RenderComponent>(cube, shader, color);
        // Add bounds for BVH - cube vertices are -0.5 to 0.5
        registry.emplace<ECS::BoundsComponent>(cube,
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f));
        return cube;
    }

    // Generate random float in range
    float randomFloat(float min, float max) {
        return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    // Generate random color
    glm::vec3 randomColor() {
        return glm::vec3(randomFloat(0.2f, 1.0f), randomFloat(0.2f, 1.0f), randomFloat(0.2f, 1.0f));
    }

public:
    TestScene(Shader* shader) : Scene() {
        ShaderManager::getInstance().setShader("shader1", shader);
        setup();

        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Create main cube at origin (red)
        mainCube = createCube(shader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // Create 10 random cubes scattered around the scene
        for (int i = 0; i < 10; ++i) {
            glm::vec3 position(
                randomFloat(-10.0f, 10.0f),
                randomFloat(-5.0f, 5.0f),
                randomFloat(-10.0f, 10.0f)
            );
            createCube(shader, position, randomColor());
        }
    }

    void setup() override {
    }

    int handleInput(Window* window) override {
        if (IO::IOSystem::getInstance().wasKeyJustPressed(IO::Key::Escape)) {
            window->setCursorMode(CursorMode::Normal);
            GameState::getInstance().CURSOR_FOCUS_STATUS = true;
        }

        // Spawn new cube with Enter key (only on just pressed to avoid spam)
        if (IO::IOSystem::getInstance().wasKeyJustPressed(IO::Key::Enter)) {
            // Spawn at random position with random color
            glm::vec3 position(
                randomFloat(-10.0f, 10.0f),
                randomFloat(-5.0f, 5.0f),
                randomFloat(-10.0f, 10.0f)
            );
            createCube(ShaderManager::getInstance().getShader("shader1"), position, randomColor());
        }

        // Move the main cube with arrow keys
        float deltaTime = GameState::getInstance().deltaTime;
        auto* transform = registry.get<ECS::TransformComponent>(mainCube);
        if (transform) {
            if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::Up)) {
                transform->position.y += deltaTime * 0.5f;
            }
            if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::Down)) {
                transform->position.y -= deltaTime * 0.5f;
            }
            if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::Left)) {
                transform->position.x -= deltaTime * 0.5f;
            }
            if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::Right)) {
                transform->position.x += deltaTime * 0.5f;
            }
        }

        bool dirty = false;
        if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::W)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::FORWARD, deltaTime));
            dirty = true;
        }
        if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::S)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::BACKWARD, deltaTime));
            dirty = true;
        }
        if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::A)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::LEFT, deltaTime));
            dirty = true;
        }
        if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::D)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::RIGHT, deltaTime));
            dirty = true;
        }
        if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::Space)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::UP, deltaTime));
            dirty = true;
        }
        if (IO::IOSystem::getInstance().isKeyPressed(IO::Key::LeftShift)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::DOWN, deltaTime));
            dirty = true;
        }
        return dirty;
    }
};

#endif //GAMEENGINE_TESTSCENE_H
