//
// Created by eceva on 1/12/2025.
//

#ifndef GAMEENGINE_MOUSEHANDLER_H
#define GAMEENGINE_MOUSEHANDLER_H

#include "../patterns/Singleton.h"

enum MouseCursorState {
    IN_WINDOW,
    OUT_OF_WINDOW,
};

class MouseHandler : public Singleton<MouseHandler> {
private:
    MouseCursorState state;
public:
    MouseHandler() : Singleton<MouseHandler>() {
//        EventDispatcher::getInstance().registerListener<>()
    }

    void changeMouseMode(MouseCursorState state) {
        this->state = state;
    }

    MouseCursorState getMouseCursorState() {
        return this->state;
    }
};

#endif //GAMEENGINE_MOUSEHANDLER_H
