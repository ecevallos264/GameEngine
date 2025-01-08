//
// Created by eceva on 1/6/2025.
//

#ifndef GAMEENGINE_KEYEVENT_H
#define GAMEENGINE_KEYEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"

struct KeyEvent : public Event {
    GLFWwindow* window;
    int keyCode;
    int action;
    float deltaTime;

    KeyEvent(
            GLFWwindow* window,
            int keycode,
            int action,
            float deltaTime) :
            window(window),
            keyCode(keycode),
            action(action),
            Event(deltaTime) {}
};

#endif //GAMEENGINE_KEYEVENT_H
