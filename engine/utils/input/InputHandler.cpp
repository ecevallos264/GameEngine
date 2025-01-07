//
// Created by eceva on 1/3/2025.
//
#include "InputHandler.h"
#include <memory>

bool InputHandler::isKeyActive(int key) {
    auto& keyState = InputHandler::getInstance().keyState;
    auto it = keyState.find(key);
    if (it != keyState.end()) {
        return it->second->state;
    }
    return false;
}

bool InputHandler::isKeyDirty(int key) {
    auto& keyState = InputHandler::getInstance().keyState;
    auto it = keyState.find(key);
    if (it != keyState.end()) {
        return it->second->isDirty;
    }
    return false;
}

void InputHandler::setKeyState(int key, bool state) {
    auto& keyState = InputHandler::getInstance().keyState;
    auto it = keyState.find(key);
    if (it == keyState.end()) {
        keyState[key] = std::make_unique<KeyState>();
    }
    keyState[key]->state = state;
    keyState[key]->isDirty = false;
}

void InputHandler::setKeyDirty(int key) {
    InputHandler::getInstance().keyState[key]->isDirty = true;
}

