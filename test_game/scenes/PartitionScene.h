//
// Created by eceva on 2/25/2025.
//

#ifndef PARTITIONSCENE_H
#define PARTITIONSCENE_H
#include "../entities/MyCube.h"
#include "../../engine/rendering/Scene.h"
#include "../../engine/core/shaders/shader-compiler.h"
#include "../../engine/core/patterns/Singleton.h"
#include "../../engine/core/shaders/ShaderManager.h"

class PartitionScene : public Scene {
public:
    PartitionScene(Shader* shader) : Scene() {
        ShaderManager::getInstance().setShader("shader1", shader);
//        MyCube* cube = new MyCube("cube1", glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//        this->addEntity(cube, "cube1");

        std::srand(static_cast<unsigned>(std::time(0)));

        // Add 10 cubes at random positions within a 10x10x10 volume
        for (int i = 0; i < 10000; i++) {
            glm::vec3 randomPos(
                    static_cast<float>(std::rand() % 11 - 5),  // X position: -5 to +5
                    static_cast<float>(std::rand() % 11 - 5),  // Y position: -5 to +5
                    static_cast<float>(std::rand() % 11 - 5)   // Z position: -5 to +5
            );

            MyCube* cube = new MyCube("cube" + std::to_string(i + 1), randomPos, glm::vec3(1.0f, 0.0f, 0.0f));
            this->addEntity(cube, "cube" + std::to_string(i + 1));
        }
    }

    void setup() override {

    }

    int handleInput(GLFWwindow* window) override {
        return 0;
    }
};

#endif //PARTITIONSCENE_H
