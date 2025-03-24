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

//        Player* player = new Player();
//        this->addEntity(player, "Player");


        MyCube* cube1 = new MyCube("Main Cube", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 0, 0));

        std::srand(static_cast<unsigned>(std::time(0)));

        // Add 10 cubes at random positions within a 10x10x10 volume
        for (int i = 0; i < 1000; i++) {
            glm::vec3 randomPos(
                    static_cast<float>(std::rand() % 201 - 100),  // X position: -5 to +5
                    static_cast<float>(std::rand() % 201 - 100),  // Y position: -5 to +5
                    static_cast<float>(std::rand() % 201 - 100)   // Z position: -5 to +5
            );

            MyCube* cube = new MyCube("cube" + std::to_string(i + 1), randomPos, glm::vec3(1.0f, 0.0f, 0.0f));
            this->addEntity(cube, "cube" + std::to_string(i + 1));
        }

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
        for(int i = 3; i < 10; i++) {
            this->addEntity(new MyCube("Cube " + std::to_string(i), glm::vec3(i*2, i*2, 0), glm::vec3(1, 0, 0)), "Cube " + std::to_string(i));
        }

//        auto* prism = new HexagonalPrism("Hexagonal Prism", glm::vec3(-1, 1, -1), glm::vec3(0));
//        prism->setOnUpdateCallback([prism](double deltaTime) {
//            prism->flags->set(EntityFlags::ENTITY_ROTATED);
//            prism->setRotation({sin(glfwGetTime()) * 100, sin(glfwGetTime()) * 100, cos(glfwGetTime()) * 100});
//            return 1;
//        });
//        this->addEntity(prism, "Hex Prism");
    }

    void setup() override {
    }

    int handleInput(GLFWwindow* window) {
        if (InputHandler::isKeyActive(GLFW_KEY_ESCAPE)) {
            MouseHandler::getInstance().changeMouseMode(MouseCursorState::OUT_OF_WINDOW);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GameState::getInstance().CURSOR_FOCUS_STATUS = true;
        }

        if(InputHandler::isKeyActive(GLFW_KEY_ENTER)) {
            MyCube* cube1 = new MyCube("Main Cube", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 0, 0));
            cube1->fixed = false;
            this->addEntity(cube1, "Main Cube");
        }

        bool dirty = false;
        if (InputHandler::isKeyActive(GLFW_KEY_W)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::FORWARD, GameState::getInstance().deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_S)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::BACKWARD, GameState::getInstance().deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_A)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::LEFT, GameState::getInstance().deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_D)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::RIGHT, GameState::getInstance().deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_SPACE)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::UP, GameState::getInstance().deltaTime));
            dirty = true;
        }
        if (InputHandler::isKeyActive(GLFW_KEY_LEFT_SHIFT)) {
            EventDispatcher::getInstance().dispatch(
                    CameraKeyMovementEvent(CameraMovementDirection::DOWN, GameState::getInstance().deltaTime));
            dirty = true;
        }
        return dirty;
    }
};

#endif //GAMEENGINE_TESTSCENE_H
