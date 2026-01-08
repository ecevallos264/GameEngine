#ifndef GAMEENGINE_SCRIPTED_SCENE_H
#define GAMEENGINE_SCRIPTED_SCENE_H

#include "../../engine/rendering/Scene.h"
#include "../../engine/scripting/ScriptComponent.h"
#include "../../engine/ecs/components/TransformComponent.h"
#include "../../engine/ecs/components/MeshComponent.h"
#include "../../engine/ecs/components/RenderComponent.h"
#include "../../engine/ecs/components/BoundsComponent.h"
#include "../../engine/core/shaders/shader-compiler.h"

// Example scene demonstrating how scripts are attached to entities
class ScriptedScene : public Scene {
private:
    Shader* shader;
    std::string scriptsPath;

    // Helper to create a cube with all required components
    std::vector<Vertex> createCubeVertices(const glm::vec3& color, float alpha = 1.0f) {
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

    std::vector<unsigned int> getCubeIndices() {
        return {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 3, 7, 7, 4, 0,
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0
        };
    }

    ECS::Entity createCube(const glm::vec3& position, const glm::vec3& color) {
        ECS::Entity cube = createEntity();
        registry.emplace<ECS::TransformComponent>(cube, position);
        registry.emplace<ECS::MeshComponent>(cube, createCubeVertices(color), getCubeIndices());
        registry.emplace<ECS::RenderComponent>(cube, shader, color);
        registry.emplace<ECS::BoundsComponent>(cube,
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f));
        return cube;
    }

public:
    ScriptedScene(Shader* shader, const std::string& scriptsPath)
        : Scene(), shader(shader), scriptsPath(scriptsPath) {
        setup();
    }

    void setup() override {
        // === Player Entity ===
        // A cube controlled by PlayerController.lua
        {
            ECS::Entity player = createCube(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.8f, 0.2f));

            // Attach script component
            auto& script = registry.emplace<ECS::ScriptComponent>(player, scriptsPath + "/PlayerController.lua");

            // Set custom properties accessible from Lua
            script.set("moveSpeed", 5.0f);
            script.set("jumpForce", 8.0f);
        }

        // === Rotating Cube ===
        // A cube that spins using Rotator.lua
        {
            ECS::Entity rotator = createCube(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.2f, 0.2f));

            auto& script = registry.emplace<ECS::ScriptComponent>(rotator, scriptsPath + "/Rotator.lua");
            script.set("rotationSpeed", 90.0f);  // 90 degrees per second
            script.set("axis", std::string("Y"));
        }

        // === Floating Cube ===
        // A cube that bobs up and down using FloatingCube.lua
        {
            ECS::Entity floater = createCube(glm::vec3(-3.0f, 2.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.8f));

            auto& script = registry.emplace<ECS::ScriptComponent>(floater, scriptsPath + "/FloatingCube.lua");
            script.set("amplitude", 1.5f);
            script.set("frequency", 1.0f);
        }

        // === Spawner Entity ===
        // Invisible entity that spawns cubes on keypress
        {
            ECS::Entity spawner = registry.create();
            registry.emplace<ECS::TransformComponent>(spawner, glm::vec3(0.0f, 5.0f, -5.0f));

            auto& script = registry.emplace<ECS::ScriptComponent>(spawner, scriptsPath + "/SpawnOnKey.lua");
            script.set("maxSpawns", 20);
        }

        // === Ground plane (no script) ===
        {
            ECS::Entity ground = createCube(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
            auto* transform = registry.get<ECS::TransformComponent>(ground);
            if (transform) {
                transform->scale = glm::vec3(20.0f, 0.2f, 20.0f);
            }
        }
    }

    int handleInput(Window* window) override {
        (void)window;
        // Input handling is done through scripts now
        return 0;
    }
};

#endif //GAMEENGINE_SCRIPTED_SCENE_H
