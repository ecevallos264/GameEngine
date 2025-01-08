//
// Created by eceva on 1/7/2025.
//

#ifndef GAMEENGINE_MOUSESTATE_H
#define GAMEENGINE_MOUSESTATE_H

#include "../patterns/Singleton.h"

class KeyState : public Singleton<KeyState> {
protected:
    double xPos = 0;
    double yPos = 0;

public:
    KeyState() : Singleton<KeyState>() {}

    static double getMousePosX() {
        return KeyState::getInstance().xPos;
    }

    static double getMousePosY() {
        return KeyState::getInstance().yPos;
    }

    static void setMousePosX(double x) {
        KeyState::getInstance().xPos = x;
    }

    static void setMousePosY(double y) {
        KeyState::getInstance().yPos = y;
    }
};

#endif //GAMEENGINE_MOUSESTATE_H
