//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_COLLISIONHANDLER_H
#define GAMEENGINE_COLLISIONHANDLER_H

#include "../core/patterns/Singleton.h"
#include "../rendering/Entity.h"
#include "../entitities/shape.h"
#include "GJK/GJK.h"

class CollisionHandler : public Singleton<CollisionHandler> {
public:
    CollisionHandler() : Singleton<CollisionHandler>() {}

    bool handleCollision(std::shared_ptr<Shape> entityA, std::shared_ptr<Shape> entityB) {
        return GJK(entityA, entityB);
    }
};

#endif //GAMEENGINE_COLLISIONHANDLER_H
