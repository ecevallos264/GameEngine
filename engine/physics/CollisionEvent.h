//
// Created by eceva on 1/17/2025.
//

#ifndef GAMEENGINE_COLLISIONEVENT_H
#define GAMEENGINE_COLLISIONEVENT_H

#include "../core/eventing/Event.h"
#include "../rendering/Entity.h"


struct CollisionEvent : public Event {
    Entity* entityA;
    Entity* entityB;
    CollisionEvent(double deltaTime, Entity* entityA, Entity* entityB) : Event(deltaTime), entityA(entityA), entityB(entityB) {}
};

#endif //GAMEENGINE_COLLISIONEVENT_H
