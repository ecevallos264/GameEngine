//
// Created by eceva on 1/8/2025.
//

#ifndef GAMEENGINE_CURSOREVENT_H
#define GAMEENGINE_CURSOREVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../utils/state/game_state.h"

struct CursorEvent : public Event {
    GLFWwindow* window;
    double x;
    double y;


    CursorEvent(
            GLFWwindow* window,
            double x, double y) :
            window(window),
            x(x),
            y(y),
            Event(GameState::getInstance().deltaTime) {}
};

#endif //GAMEENGINE_CURSOREVENT_H
