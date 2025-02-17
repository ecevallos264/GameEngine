// Player.h
//
// Created by eceva on 1/7/2025.
//

#ifndef GAMEENGINE_PLAYER_H
#define GAMEENGINE_PLAYER_H

#include <iostream>
#include "../../engine/core/eventing/EventListener.h"
#include "../../engine/core/eventing/events/KeyEvent.h"
#include "../../engine/core/state/game_state.h"
#include "../../engine/input/InputHandler.h"
#include "../../engine/core/eventing/events/CursorEvent.h"
#include "../../engine/core/eventing/events/CameraKeyMovementEvent.h"
#include "../../engine/input/MouseHandler.h"
#include "../../engine/rendering/Entity.h"
#include "../../engine/entitities/Shape.h"

class Player : public EventListener, public Shape {
public:
    Player(Shader* shader) : EventListener(), Shape(shader) {
        EventDispatcher::getInstance().registerListener<KeyEvent>([this](const Event& event) {
            this->onEvent(static_cast<const KeyEvent&>(event));
        });
        this->boundingBox = BoundingBox::fromPoints(shader, {
                glm::vec3(1.0f),
                glm::vec3(-1.0f),
        });
    }

    void onEvent(const Event &event) {
    }

    void onEvent(const KeyEvent &event) {
        if (InputHandler::isKeyActive(GLFW_KEY_ESCAPE)) {
            MouseHandler::getInstance().changeMouseMode(MouseCursorState::OUT_OF_WINDOW);
            glfwSetInputMode(event.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GameState::getInstance().CURSOR_FOCUS_STATUS = true;
        }
    }

    void onEvent(const CursorEvent &event) {
        //...
    }

    int update(float deltaTime) override {
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

    std::type_index getType() {
        return typeid(Player);
    }
};

#endif //GAMEENGINE_PLAYER_H