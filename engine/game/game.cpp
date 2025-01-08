#include "game.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "../camera/camera.h"
#include "../utils/patterns/Singleton.h"
#include "../utils/settings/settings.h"
#include "../entitities/EntityHandler.h"
#include "../utils/state/game_state.h"
#include "../eventing/EventDispatcher.h"
#include "../entitities/line.h"

void Game::renderGame(GLFWwindow* window) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->shader->use();

    glm::mat4 view = glm::lookAt(
            Camera::getInstance().getPosition(),
            Camera::getInstance().getPosition() + Camera::getInstance().getFront(),
            Camera::getInstance().getUp()
    );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Settings::WINDOW_WIDTH / (float)Settings::WINDOW_HEIGHT, 0.1f, Settings::MAX_RENDER_DISTANCE);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for(int i = 0; i < EntityHandler::getInstance().getEntities().size(); i++) {
        if(EntityHandler::getInstance().entities[i]->isDestroyed()) {
            EntityHandler::getInstance().entities.erase(EntityHandler::getInstance().entities.begin() + i);
        }
        EntityHandler::getInstance().entities[i]->update(GameState::getInstance().deltaTime, shader);
        EntityHandler::getInstance().entities[i]->draw();
    }
}



void Game::renderGrid() {
    float unit = 1.0f;

    EntityHandler::getInstance().addEntity(new Line(glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f), glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f));
    EntityHandler::getInstance().addEntity(new Line(glm::vec3(0.0f, Settings::MAX_RENDER_DISTANCE, 0.0f), glm::vec3(0.0f, -Settings::MAX_RENDER_DISTANCE, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f));
    EntityHandler::getInstance().addEntity(new Line(glm::vec3(0.0f, 0.0f, Settings::MAX_RENDER_DISTANCE), glm::vec3(0.0f, 0.0f, -Settings::MAX_RENDER_DISTANCE), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f));

    for(float i = -Settings::MAX_RENDER_DISTANCE * 1/unit; i <= Settings::MAX_RENDER_DISTANCE * 1/unit; i++) {
        if(i == 0) continue;
        float line = i * unit;
        EntityHandler::getInstance().addEntity(new Line(glm::vec3(Settings::MAX_RENDER_DISTANCE, 0.0f, line), glm::vec3(-Settings::MAX_RENDER_DISTANCE, 0.0f, line), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f));
    }

    for(float i = -Settings::MAX_RENDER_DISTANCE * 1/unit; i <= Settings::MAX_RENDER_DISTANCE * 1/unit; i++) {
        if(i == 0) continue;
        float line = i * unit;
        EntityHandler::getInstance().addEntity(new Line(glm::vec3(line, 0.0f,Settings::MAX_RENDER_DISTANCE), glm::vec3(line, 0.0f, -Settings::MAX_RENDER_DISTANCE), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f));
    }
}

void Game::run(GLFWwindow* window) {
    this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));
    renderGrid();
    shader->use();
//    bindKeyListener();



    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        GameState::getInstance().deltaTime = currentFrame - GameState::getInstance().lastFrame;
        GameState::getInstance().lastFrame = currentFrame;

        renderGame(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}