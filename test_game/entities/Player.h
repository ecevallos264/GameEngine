// Player.h
//
// Created by eceva on 1/7/2025.
//

#ifndef GAMEENGINE_PLAYER_H
#define GAMEENGINE_PLAYER_H

#include <iostream>
#include "../../engine/eventing/EventListener.h"
#include "../../engine/eventing/events/KeyEvent.h"
#include "../../engine/utils/state/game_state.h"
#include "../../engine/entitities/Entity.h"
#include "../../engine/utils/input/InputHandler.h"

class Player : public EventListener, public Entity {
public:
    Player() : EventListener() {
        EventDispatcher::getInstance().registerListener<KeyEvent>([this](const Event& event) {
            this->onEvent(static_cast<const KeyEvent&>(event));
        });
    }

    void onEvent(const Event &event) {
        std::cout << "Event was Passed" << std::endl;
    }

    void onEvent(const KeyEvent &event) {
        if (InputHandler::isKeyActive(GLFW_KEY_ESCAPE)) {
            std::cout << "Escape key pressed" << std::endl;
            glfwSetInputMode(event.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GameState::getInstance().CURSOR_FOCUS_STATUS = true;
        }
    }

    void draw() {
        return;
    }

    void update(float deltaTime, Shader* shader) override {

        if (InputHandler::isKeyActive(GLFW_KEY_W)) {
            std::cout << "W key pressed" << std::endl;
            Camera::getInstance().setPosition(
                    Camera::getInstance().getPosition() +
                    (Camera::getInstance().getCalcSpeed(GameState::getInstance().deltaTime) * Camera::getInstance().getFront()));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_S)) {
            std::cout << "S key pressed" << std::endl;
            Camera::getInstance().setPosition(
                    Camera::getInstance().getPosition() -
                    (Camera::getInstance().getCalcSpeed(GameState::getInstance().deltaTime) * Camera::getInstance().getFront()));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_A)) {
            std::cout << "A key pressed" << std::endl;
            Camera::getInstance().setPosition(
                    Camera::getInstance().getPosition() -
                    glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) *
                    Camera::getInstance().getCalcSpeed(GameState::getInstance().deltaTime));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_D)) {
            std::cout << "D key pressed" << std::endl;
            Camera::getInstance().setPosition(
                    Camera::getInstance().getPosition() +
                    glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) *
                    Camera::getInstance().getCalcSpeed(GameState::getInstance().deltaTime));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_SPACE)) {
            std::cout << "Space key pressed" << std::endl;
            Camera::getInstance().setPosition(
                    Camera::getInstance().getPosition() +
                    glm::vec3(0.0f, Camera::getInstance().getCalcSpeed(GameState::getInstance().deltaTime), 0.0f));
        }
    }
};

#endif //GAMEENGINE_PLAYER_H