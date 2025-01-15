//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERAKEYMOVEMENTEVENT_H
#define GAMEENGINE_CAMERAKEYMOVEMENTEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../state/game_state.h"

enum CameraMovementDirection {
    UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD
};

struct CameraKeyMovementEvent : public Event {
    CameraMovementDirection  direction;
    double deltaTime;

    CameraKeyMovementEvent(
            CameraMovementDirection direction,
            double deltaTime) :
            direction(direction),
            Event(deltaTime) {}
};

#endif //GAMEENGINE_CAMERAKEYMOVEMENTEVENT_H
