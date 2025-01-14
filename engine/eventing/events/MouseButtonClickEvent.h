//
// Created by eceva on 1/12/2025.
//

#ifndef GAMEENGINE_MOUSEBUTTONCLICKEVENT_H
#define GAMEENGINE_MOUSEBUTTONCLICKEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../utils/patterns/Singleton.h"

struct MouseButtonClickEvent : public Event {
    GLFWwindow* window;
    int keyCode;
    int action;
    float deltaTime;

    MouseButtonClickEvent(
            GLFWwindow* window,
            int keycode,
            int action) :
            window(window),
            keyCode(keycode),
            action(action),
            Event(GameState::getInstance().deltaTime) {}
};

#endif //GAMEENGINE_MOUSEBUTTONCLICKEVENT_H
