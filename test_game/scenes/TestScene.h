//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_TESTSCENE_H
#define GAMEENGINE_TESTSCENE_H

#include "../entities/MyCube.h"
#include "../../engine/rendering/Scene.h"
#include "../../engine/entitities/Line.h"
#include "../../engine/core/shaders/shader-compiler.h"
#include "../../engine/core/patterns/Singleton.h"
#include "../../engine/core/shaders/ShaderManager.h"
#include "../../engine/physics/CollisionHandler.h"

#include "../../engine/entitities/Point.h"
#include "../entities/HexagonalPrism.h"

class TestScene : public Scene {

public:
    TestScene(Shader* shader) : Scene() {
        ShaderManager::getInstance().setShader("shader1", shader);
        setup();
        float unit = 1.0f;

        // this->addEntity(new Line(
        //         glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f),
        //         glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f),
        //         glm::vec3(1.0f, 0.0f, 0.0f),
        //         1.0f), "X Line");
        //
        // this->addEntity(new Line(
        //         glm::vec3(0.0f, Settings::MAX_RENDER_DISTANCE, 0.0f),
        //         glm::vec3(0.0f, -Settings::MAX_RENDER_DISTANCE, 0.0f),
        //         glm::vec3(0.0f, 1.0f, 0.0f),
        //         1.0f), "Y Line");
        //
        // this->addEntity(new Line(
        //         glm::vec3(0.0f, 0.0f, Settings::MAX_RENDER_DISTANCE),
        //         glm::vec3(0.0f, 0.0f, -Settings::MAX_RENDER_DISTANCE),
        //         glm::vec3(0.0f, 0.0f, 1.0f),
        //         1.0f), "Z Line");

        // for (float i = -Settings::MAX_RENDER_DISTANCE / unit; i <= Settings::MAX_RENDER_DISTANCE / unit; i++) {
        //     if (i == 0) continue;
        //     float line = i * unit;
        //     this->addEntity(new Line(
        //             glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, line),
        //             glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, line),
        //             glm::vec3(0.5f, 0.5f, 0.5f),
        //             1.0f), "Line");
        // }
        //
        // for (float i = -Settings::MAX_RENDER_DISTANCE / unit; i <= Settings::MAX_RENDER_DISTANCE / unit; i++) {
        //     if (i == 0) continue;
        //     float line = i * unit;
        //     this->addEntity(
        //             new Line(glm::vec3(
        //                     line,
        //                     0.0f,
        //                     Settings::MAX_RENDER_DISTANCE),
        //                     glm::vec3(line, 0.0f, -Settings::MAX_RENDER_DISTANCE),glm::vec3(0.5f, 0.5f, 0.5f),
        //                     1.0f), "Line");
        // }

        Player* player = new Player();
        this->addEntity(player, "Player");


        MyCube* cube1 = new MyCube("Main Cube", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 0, 0));

        cube1->fixed = false;
        cube1->setOnUpdateCallback([cube1](double deltaTime) {
            int dirty = false;
            if(InputHandler::isKeyActive(GLFW_KEY_UP)) {
                cube1->position.y += deltaTime * 0.5;
                dirty = true;
            }
            if(InputHandler::isKeyActive(GLFW_KEY_DOWN)) {
                cube1->position.y -= deltaTime * 0.5;
                dirty = true;
            }
            if(InputHandler::isKeyActive(GLFW_KEY_LEFT)) {
                cube1->position.x -= deltaTime * 0.5;
                dirty = true;

            }
            if(InputHandler::isKeyActive(GLFW_KEY_RIGHT)) {
                cube1->position.x += deltaTime * 0.5;
                dirty = true;
            }
            cube1->flags->set(EntityFlags::ENTITY_MOVED);
            return dirty;
        });
        this->addEntity(cube1, "Main Cube");
//        for(int i = 3; i < 10; i++) {
//            this->addEntity(new MyCube("Cube " + std::to_string(i), glm::vec3(i*2, i*2, 0), glm::vec3(1, 0, 0)), "Cube");
//        }

        auto* prism = new HexagonalPrism("Hexagonal Prism", glm::vec3(-1, 1, -1), glm::vec3(0));
        prism->setOnUpdateCallback([prism](double deltaTime) {
            prism->flags->set(EntityFlags::ENTITY_ROTATED);
            prism->setRotation({sin(glfwGetTime()) * 100, sin(glfwGetTime()) * 100, cos(glfwGetTime()) * 100});
            return 1;
        });
        this->addEntity(prism, "Hex Prism");
    }

    void setup() override {
        this->root = new Octree(new BoundingBox(glm::vec3(-0.5f), glm::vec3(0.5f)));
        std::cout << "root dimensions: " << this->root->region->calculateDimensions().x << ":" << this->root->region->calculateDimensions().y << ":" << this->root->region->calculateDimensions().z << std::endl;
        this->root->build();
    }
};

#endif //GAMEENGINE_TESTSCENE_H
