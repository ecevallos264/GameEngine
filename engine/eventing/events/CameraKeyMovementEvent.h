//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERAKEYMOVEMENTEVENT_H
#define GAMEENGINE_CAMERAKEYMOVEMENTEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../core/state/game_state.h"

enum CameraMovementDirection {
    UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD
};

struct CameraKeyMovementEvent : public Event {
    CameraMovementDirection  direction;
    double deltaTime;

    CameraKeyMovementEvent(
            CameraMovementDirection direction,
            double dt) :
            Event(static_cast<float>(dt)),
            direction(direction),
            deltaTime(dt) {}
};

#endif //GAMEENGINE_CAMERAKEYMOVEMENTEVENT_H
