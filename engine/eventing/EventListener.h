//
// Created by eceva on 1/6/2025.
//

#ifndef GAMEENGINE_EVENTLISTENER_H
#define GAMEENGINE_EVENTLISTENER_H

#include "Event.h"

class EventListener {
public:
    virtual void onEvent(const Event& event) = 0;
};

#endif //GAMEENGINE_EVENTLISTENER_H
