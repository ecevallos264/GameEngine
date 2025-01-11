//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERAMOVEMENTEVENT_H
#define GAMEENGINE_CAMERAMOVEMENTEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../utils/state/game_state.h"

enum CameraMovementDirection {
    LEFT,
    RIGHT,
    BACKWARD,
    FORWARD,
    UP,
    DOWN
};

struct CameraMovementEvent : public Event {
    CameraMovementDirection  direction;
    double deltaTime;

    CameraMovementEvent(
            CameraMovementDirection direction,
            double deltaTime) :
            direction(direction),
            Event(deltaTime) {}
};

#endif //GAMEENGINE_CAMERAMOVEMENTEVENT_H
