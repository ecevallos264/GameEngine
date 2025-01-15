//
// Created by eceva on 1/3/2025.
//
#include "InputHandler.h"
#include "../core/eventing/events/KeyEvent.h"
#include "../core/patterns/Singleton.h"
#include <memory>
#include "../core/eventing/EventDispatcher.h"
#include "../core/state/game_state.h"
#include "../core/eventing/events/MouseMovementEvent.h"
#include "../camera/CameraHandler.h"
#include "../core/observability/CalcTime.h"

bool InputHandler::isKeyActive(int key) {
    auto& keyState = InputHandler::getInstance().keyboardState.keyState;
    auto it = keyState.find(key);
    if (it != keyState.end()) {
        return it->second->state;
    }
    return false;
}

bool InputHandler::isKeyDirty(int key) {
    auto& keyState = InputHandler::getInstance().keyboardState.keyState;
    auto it = keyState.find(key);
    if (it != keyState.end()) {
        return it->second->isDirty;
    }
    return false;
}

void InputHandler::setKeyState(int key, bool state) {
    auto& keyState = InputHandler::getInstance().keyboardState.keyState;
    auto it = keyState.find(key);
    if (it == keyState.end()) {
        keyState[key] = std::make_unique<KeyState>();
    }
    keyState[key]->state = state;
    keyState[key]->isDirty = false;
}

void InputHandler::setKeyDirty(int key) {
    InputHandler::getInstance().keyboardState.keyState[key]->isDirty = true;
}

void InputHandler::setMouseMovementCallback(GLFWwindow* window, std::function<void(GLFWwindow *, int, int)> callback) {
    InputHandler::getInstance().onMouseMovementCallback = callback;
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        Camera* camera = CameraHandler::getInstance().getCamera();

        EventDispatcher::getInstance().dispatch(
                MouseMovementEvent(
                    x - camera->getXPosition(),
                    camera->getYPosition() - y,
                    MouseHandler::getInstance().getMouseCursorState(),
                    GameState::getInstance().deltaTime));
        camera->setXPosition(x);
        camera->setYPosition(y);
        InputHandler::getInstance().onMouseMovementCallback(window, x, y);
    });
}

void InputHandler::setKeyPressCallback(GLFWwindow* window, std::function<void(GLFWwindow *, int, int, int, int)> callback) {
    InputHandler:getInstance().onKeyPressCallback = callback;
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        KeyEvent event(window, key, action);
        if(action != GLFW_RELEASE) {
            InputHandler::setKeyState(key, true);
        } else {
            InputHandler::setKeyState(key, false);
        }
        EventDispatcher::getInstance().dispatch(event);
        InputHandler::getInstance().onKeyPressCallback(window, key, scancode, action, mods);
    });
}

