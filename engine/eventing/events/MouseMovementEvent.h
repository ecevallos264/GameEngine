#ifndef GAMEENGINE_MOUSEMOVEMENTEVENT_H
#define GAMEENGINE_MOUSEMOVEMENTEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../utils/state/game_state.h"

struct MouseMovementEvent : public Event {
    float deltaX;
    float deltaY;
    float deltaTime;

    MouseMovementEvent(
            float deltaX, float deltaY,
            float deltaTime) :
            deltaX(deltaX),
            deltaY(deltaY),
            deltaTime(deltaTime),
            Event(deltaTime) {}
};

#endif //GAMEENGINE_MOUSEMOVEMENTEVENT_H
