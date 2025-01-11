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
#include "../../engine/eventing/events/CursorEvent.h"
#include "../../engine/eventing/events/CameraMovementEvent.h"

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
            glfwSetInputMode(event.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GameState::getInstance().CURSOR_FOCUS_STATUS = true;
        }
    }

    void onEvent(const CursorEvent &event) {
        //...
    }

    void draw() {
        return;
    }

    void update(float deltaTime, Shader* shader) override {

        if (InputHandler::isKeyActive(GLFW_KEY_W)) {
            EventDispatcher::getInstance().dispatch(
                    CameraMovementEvent(CameraMovementDirection::FORWARD, GameState::getInstance().deltaTime));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_S)) {
            EventDispatcher::getInstance().dispatch(
                    CameraMovementEvent(CameraMovementDirection::BACKWARD, GameState::getInstance().deltaTime));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_A)) {
            EventDispatcher::getInstance().dispatch(
                    CameraMovementEvent(CameraMovementDirection::LEFT, GameState::getInstance().deltaTime));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_D)) {
            EventDispatcher::getInstance().dispatch(
                    CameraMovementEvent(CameraMovementDirection::RIGHT, GameState::getInstance().deltaTime));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_SPACE)) {
            EventDispatcher::getInstance().dispatch(
                    CameraMovementEvent(CameraMovementDirection::UP, GameState::getInstance().deltaTime));
        }
        if (InputHandler::isKeyActive(GLFW_KEY_LEFT_SHIFT)) {
            EventDispatcher::getInstance().dispatch(
                    CameraMovementEvent(CameraMovementDirection::DOWN, GameState::getInstance().deltaTime));
        }
    }
};

#endif //GAMEENGINE_PLAYER_H