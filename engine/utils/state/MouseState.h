//
// Created by eceva on 1/7/2025.
//

#ifndef GAMEENGINE_MOUSESTATE_H
#define GAMEENGINE_MOUSESTATE_H

#include "../patterns/Singleton.h"

enum {

};

class MouseState : public Singleton<MouseState> {
protected:
    double xPos = 0;
    double yPos = 0;

public:
    MouseState() : Singleton<MouseState>() {}

    static double getMousePosX() {
        return MouseState::getInstance().xPos;
    }

    static double getMousePosY() {
        return MouseState::getInstance().yPos;
    }

    static void setMousePosX(double x) {
        MouseState::getInstance().xPos = x;
    }

    static void setMousePosY(double y) {
        MouseState::getInstance().yPos = y;
    }
};

#endif //GAMEENGINE_MOUSESTATE_H
