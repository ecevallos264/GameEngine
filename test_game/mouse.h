//
// Created by eceva on 1/5/2025.
//

#ifndef GAMEENGINE_MOUSE_H
#define GAMEENGINE_MOUSE_H

#include <glfw/glfw3.h>
#include "functional"
#include "../engine/utils/state/MouseState.h"

void onAnyMouseMovementAction(std::function<void()> userActionCallback) {
//    Record new mouse metrics (new cursor pos, isMouseInWindow)
    MouseState::setMousePosX(0);
    MouseState::setMousePosY(0);

    if(userActionCallback) {
        userActionCallback();
    }
}

#endif //GAMEENGINE_MOUSE_H
