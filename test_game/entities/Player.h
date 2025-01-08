//
// Created by eceva on 1/7/2025.
//

#ifndef GAMEENGINE_PLAYER_H
#define GAMEENGINE_PLAYER_H

#include <iostream>
#include "../../engine/eventing/EventListener.h"
#include "../../engine/eventing/events/KeyEvent.h"
#include "../../engine/utils/state/game_state.h"

class Player : public EventListener {
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
        std::cout << "Delta Time: " << event.deltaTime << ":" << GameState::getInstance().deltaTime << std::endl;
        switch (event.keyCode) {
            case GLFW_KEY_ESCAPE: {
                glfwSetInputMode(event.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                GameState::getInstance().CURSOR_FOCUS_STATUS = true;
                break;
            }
            case GLFW_KEY_W: {
                if (event.action != GLFW_RELEASE) {
                    Camera::getInstance().setPosition(
                            Camera::getInstance().getPosition() +
                            (Camera::getInstance().getCalcSpeed(event.deltaTime) * Camera::getInstance().getFront()));
                }
                break;
            }
            case GLFW_KEY_S: {
                if (event.action == GLFW_PRESS) {
                    Camera::getInstance().setPosition(
                            Camera::getInstance().getPosition() -
                            (Camera::getInstance().getCalcSpeed(event.deltaTime) * Camera::getInstance().getFront()));
                }
                break;
            }
            case GLFW_KEY_A: {
                if (event.action == GLFW_PRESS) {
                    Camera::getInstance().setPosition(
                            Camera::getInstance().getPosition() -
                            glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) *
                            Camera::getInstance().getCalcSpeed(event.deltaTime));
                }
                break;
            }
            case GLFW_KEY_D: {
                if (event.action == GLFW_PRESS) {
                    Camera::getInstance().setPosition(
                            Camera::getInstance().getPosition() +
                            glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) *
                            Camera::getInstance().getCalcSpeed(event.deltaTime));
                }
                break;
            }
            case GLFW_KEY_SPACE: {
                if (event.action == GLFW_PRESS) {
                    Camera::getInstance().setPosition(
                            Camera::getInstance().getPosition() +
                            glm::vec3(0.0f, Camera::getInstance().getCalcSpeed(event.deltaTime), 0.0f));
                }
                break;
            }
            default:
                break;
        }
    }
};

#endif //GAMEENGINE_PLAYER_H
