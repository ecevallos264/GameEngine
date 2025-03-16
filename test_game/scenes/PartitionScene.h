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
        MyCube* cube = new MyCube("cube1", glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        this->addEntity(cube, "cube1");
        root = new Octree();
        root->addToPending(cube);
    }

    void setup() override {

    }

    int handleInput(GLFWwindow* window) override {
        return 0;
    }
};

#endif //PARTITIONSCENE_H
