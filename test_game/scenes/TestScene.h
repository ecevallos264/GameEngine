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

class TestScene : public Scene {
private:
    Shader* shader;

public:
    TestScene(Shader* shader) : Scene() {
        this->shader = shader;
        float unit = 1.0f;

        entityController->addEntity(std::make_shared<Line>(
                glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f),
                glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                1.0f, this->shader));

        entityController->addEntity(std::make_shared<Line>(
                glm::vec3(0.0f, Settings::MAX_RENDER_DISTANCE, 0.0f),
                glm::vec3(0.0f, -Settings::MAX_RENDER_DISTANCE, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                1.0f, this->shader));

        entityController->addEntity(std::make_shared<Line>(
                glm::vec3(0.0f, 0.0f, Settings::MAX_RENDER_DISTANCE),
                glm::vec3(0.0f, 0.0f, -Settings::MAX_RENDER_DISTANCE),
                glm::vec3(0.0f, 0.0f, 1.0f),
                1.0f, this->shader));

        for (float i = -Settings::MAX_RENDER_DISTANCE / unit; i <= Settings::MAX_RENDER_DISTANCE / unit; i++) {
            if (i == 0) continue;
            float line = i * unit;
            entityController->addEntity(std::make_shared<Line>(
                    glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, line),
                    glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, line),
                    glm::vec3(0.5f, 0.5f, 0.5f),
                    1.0f,
                    this->shader));
        }

        for (float i = -Settings::MAX_RENDER_DISTANCE / unit; i <= Settings::MAX_RENDER_DISTANCE / unit; i++) {
            if (i == 0) continue;
            float line = i * unit;
            entityController->addEntity(std::make_shared<Line>(
                    glm::vec3(line, 0.0f, Settings::MAX_RENDER_DISTANCE),
                    glm::vec3(line, 0.0f, -Settings::MAX_RENDER_DISTANCE),
                    glm::vec3(0.5f, 0.5f, 0.5f),
                    1.0f,
                    this->shader));
        }
        std::shared_ptr<Player> player = std::make_shared<Player>(this->shader);
        entityController->addEntity(player);

        entityController->addEntity(std::make_shared<Cube>(glm::vec3(-2.0f, 0.0f, -2.0f), this->shader));

    }
};

#endif //GAMEENGINE_TESTSCENE_H
