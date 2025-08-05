//
// Created by eceva on 4/30/2025.
//
#include "../Event.h"
#include "../../state/game_state.h"
#include "CameraKeyMovementEvent.h"

#ifndef GAMEENGINE_DEBUGCAMERAKEYMOVEMENTEVENT_H
#define GAMEENGINE_DEBUGCAMERAKEYMOVEMENTEVENT_H


struct DebugCameraKeyMovementEvent : public Event {
    CameraMovementDirection  direction;
    double deltaTime;

    DebugCameraKeyMovementEvent(
            CameraMovementDirection direction,
            double deltaTime) :
            direction(direction),
            Event(deltaTime) {}
};

#endif //GAMEENGINE_DEBUGCAMERAKEYMOVEMENTEVENT_H
