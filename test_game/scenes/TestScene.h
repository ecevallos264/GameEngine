//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_TESTSCENE_H
#define GAMEENGINE_TESTSCENE_H

#include <memory>
#include "../../engine/rendering/Scene.h"
#include "../../engine/entitities/line.h"
#include "../../engine/core/settings/settings.h"
#include "../../engine/core/shaders/shader-compiler.h"
#include "../../engine/entitities/Cube.h"
#include "../../engine/physics/GJK/GJK.h"
#include "../../engine/core/patterns/Singleton.h"
#include "../../engine/physics/CollisionHandler.h"
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


        Cube* cube1 = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), this->shader, glm::vec3(1, 0, 0.0f));
        Cube* cube2 = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), this->shader, glm::vec3(1, 0, 0.0f));
        cube1->fixed = false;
        cube2->fixed = false;
        cube1->setOnUpdateCallback([cube1](double deltaTime) {
            cube1->position.x += sin(glfwGetTime()) / 30;
            cube1->position.y += cos(glfwGetTime()) / 30;
        });
        cube1->fixed = true;
        cube2->fixed = true;
        entityController->addEntity(cube1);
        entityController->addEntity(cube2);


    }
};

#endif //GAMEENGINE_TESTSCENE_H
