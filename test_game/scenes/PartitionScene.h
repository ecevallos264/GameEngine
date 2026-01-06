//
// Created by eceva on 2/25/2025.
//

#ifndef PARTITIONSCENE_H
#define PARTITIONSCENE_H

#include "../../engine/rendering/Scene.h"
#include "../../engine/core/shaders/shader-compiler.h"
#include "../../engine/core/patterns/Singleton.h"
#include "../../engine/core/shaders/ShaderManager.h"
#include "../../engine/ecs/ECS.h"
#include "TestScene.h"  // For CubeMesh helper

class PartitionScene : public Scene {
public:
    PartitionScene(Shader* shader) : Scene() {
        ShaderManager::getInstance().setShader("shader1", shader);

        std::srand(static_cast<unsigned>(std::time(0)));

        // Add 10000 cubes at random positions within a 10x10x10 volume
        for (int i = 0; i < 10000; i++) {
            glm::vec3 randomPos(
                    static_cast<float>(std::rand() % 11 - 5),  // X position: -5 to +5
                    static_cast<float>(std::rand() % 11 - 5),  // Y position: -5 to +5
                    static_cast<float>(std::rand() % 11 - 5)   // Z position: -5 to +5
            );

            ECS::Entity cube = createEntity();
            registry.emplace<ECS::TransformComponent>(cube, randomPos);
            registry.emplace<ECS::MeshComponent>(cube,
                CubeMesh::createVertices(glm::vec3(1.0f, 0.0f, 0.0f)),
                CubeMesh::getIndices());
            registry.emplace<ECS::RenderComponent>(cube, shader, glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }

    void setup() override {
    }

    int handleInput(Window* window) override {
        (void)window;
        return 0;
    }
};

#endif //PARTITIONSCENE_H
