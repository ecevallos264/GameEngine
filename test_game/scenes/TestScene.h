//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_TESTSCENE_H
#define GAMEENGINE_TESTSCENE_H

#include <memory>
#include "../entities/MyCube.h"
#include "../../engine/rendering/Scene.h"
#include "../../engine/entitities/line.h"
#include "../../engine/core/settings/settings.h"
#include "../../engine/core/shaders/shader-compiler.h"
#include "../../engine/entitities/Cube.h"
#include "../../engine/physics/GJK/GJK.h"
#include "../../engine/core/patterns/Singleton.h"
#include "../../engine/physics/CollisionHandler.h"

#include "../../engine/entitities/Point.h"

class TestScene : public Scene {
private:
    Shader* shader;

public:
    TestScene(Shader* shader) : Scene() {
        this->shader = shader;
        float unit = 1.0f;

        entityController->addEntity(new Line(
                glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f),
                glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                1.0f, this->shader));

        entityController->addEntity(new Line(
                glm::vec3(0.0f, Settings::MAX_RENDER_DISTANCE, 0.0f),
                glm::vec3(0.0f, -Settings::MAX_RENDER_DISTANCE, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                1.0f, this->shader));

        entityController->addEntity(new Line(
                glm::vec3(0.0f, 0.0f, Settings::MAX_RENDER_DISTANCE),
                glm::vec3(0.0f, 0.0f, -Settings::MAX_RENDER_DISTANCE),
                glm::vec3(0.0f, 0.0f, 1.0f),
                1.0f, this->shader));

        for (float i = -Settings::MAX_RENDER_DISTANCE / unit; i <= Settings::MAX_RENDER_DISTANCE / unit; i++) {
            if (i == 0) continue;
            float line = i * unit;
            entityController->addEntity(new Line(
                    glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, line),
                    glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, line),
                    glm::vec3(0.5f, 0.5f, 0.5f),
                    1.0f,
                    this->shader
                    ));
        }

        for (float i = -Settings::MAX_RENDER_DISTANCE / unit; i <= Settings::MAX_RENDER_DISTANCE / unit; i++) {
            if (i == 0) continue;
            float line = i * unit;
            entityController->addEntity(
                    new Line(glm::vec3(
                            line,
                            0.0f,
                            Settings::MAX_RENDER_DISTANCE),
                            glm::vec3(line, 0.0f, -Settings::MAX_RENDER_DISTANCE),glm::vec3(0.5f, 0.5f, 0.5f),
                            1.0f,
                            this->shader));
        }

        Player* player = new Player(this->shader);
        entityController->addEntity(player);


        MyCube* cube1 = new MyCube(glm::vec3(5.0f, 0.0f, 0.0f), this->shader, glm::vec3(1, 0, 0));

        cube1->fixed = false;
        cube1->setOnUpdateCallback([cube1](double deltaTime) {
            if(InputHandler::getInstance().isKeyActive(GLFW_KEY_UP)) {
                cube1->position.y += deltaTime * 1;

            }
            if(InputHandler::getInstance().isKeyActive(GLFW_KEY_DOWN)) {
                cube1->position.y -= deltaTime * 1;
            }
            if(InputHandler::getInstance().isKeyActive(GLFW_KEY_LEFT)) {
                cube1->position.x += deltaTime * 1;

            }
            if(InputHandler::getInstance().isKeyActive(GLFW_KEY_RIGHT)) {
                cube1->position.x -= deltaTime * 1;
            }
        });
        entityController->addEntity(cube1);
        for(int i = 0; i < 1; i++) {
            MyCube* cube2 = new MyCube(glm::vec3(i*2, i*2, 0), this->shader, glm::vec3(1, 0, 0));

            cube2->fixed = false;
            entityController->addEntity(cube2);
        }
    }
};

#endif //GAMEENGINE_TESTSCENE_H
