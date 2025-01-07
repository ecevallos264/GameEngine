//
// Created by eceva on 1/5/2025.
//

#ifndef GAMEENGINE_KEY_H
#define GAMEENGINE_KEY_H

#include <glfw/glfw3.h>
#include "InputHandler.h"

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        InputHandler::setKeyState(key, true);
    } else if (action == GLFW_RELEASE) {
        InputHandler::setKeyDirty(key);
        InputHandler::setKeyState(key, false);
    }
}

#endif //GAMEENGINE_KEY_H
