//
// Created by eceva on 1/6/2025.
//

#ifndef GAMEENGINE_KEYEVENT_H
#define GAMEENGINE_KEYEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"

struct KeyEvent : public Event {
    int keyCode;
    GLFWwindow* window;
    KeyEvent(float deltaTime, int keyCode, GLFWwindow* window) : keyCode(keyCode), window(window), Event(deltaTime) {}
};

#endif //GAMEENGINE_KEYEVENT_H
