#ifndef GAMEENGINE_MOUSEMOVEMENTEVENT_H
#define GAMEENGINE_MOUSEMOVEMENTEVENT_H

#include <glfw/glfw3.h>
#include "../Event.h"
#include "../../state/game_state.h"
#include "MouseMovementEvent.h"
#include "../../types/MouseState.h"

struct MouseMovementEvent : public Event {
    double deltaX;
    double deltaY;
    float deltaTime;
    MouseCursorState state;

    MouseMovementEvent(double deltaX, double deltaY, MouseCursorState state, float deltaTime) :
            deltaX(deltaX),
            deltaY(deltaY),
            state(state),
            deltaTime(deltaTime),
            Event(deltaTime) {}
};

#endif //GAMEENGINE_MOUSEMOVEMENTEVENT_H
