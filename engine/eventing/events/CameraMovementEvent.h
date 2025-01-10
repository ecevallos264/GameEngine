//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERAMOVEMENTEVENT_H
#define GAMEENGINE_CAMERAMOVEMENTEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../utils/state/game_state.h"

struct CameraMovementEvent : public Event {
    GLFWwindow* window;


    CameraMovementEvent(
            GLFWwindow* window,
            double x, double y) :
            window(window),
            Event(GameState::getInstance().deltaTime) {}
};

#endif //GAMEENGINE_CAMERAMOVEMENTEVENT_H
