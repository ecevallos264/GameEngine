#ifndef GAMEENGINE_MOUSEMOVEMENTEVENT_H
#define GAMEENGINE_MOUSEMOVEMENTEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../utils/state/game_state.h"

struct MouseMovementEvent : public Event {
    float xPosition;
    float yPosition;
    float prevXPosition;
    float prevYPosition;
    float deltaTime;

    MouseMovementEvent(
            float xPosition, float yPosition,
            float prevXPosition, float prevYPosition,
            float deltaTime) :
            xPosition(xPosition), yPosition(yPosition),
            prevXPosition(prevXPosition), prevYPosition(prevYPosition),
            deltaTime(deltaTime),
            Event(deltaTime) {}
};

#endif //GAMEENGINE_MOUSEMOVEMENTEVENT_H
