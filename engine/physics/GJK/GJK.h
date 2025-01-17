//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_GJK_H
#define GAMEENGINE_GJK_H

#include <memory>
#include "glm/vec3.hpp"
#include "../Simplex.h"
#include "../../entitities/shape.h"

enum CollisionState {
    EVOLVING,
    COLLISION,
    NO_COLLISION
};

extern glm::vec3 minkowskiSupport(Entity* shapeA, Entity* shapeB, const glm::vec3& direction);

extern CollisionState GJK(Entity* shapeA, Entity* shapeB);

extern bool check(Entity* shapeA, Entity* shapeB);



#endif //GAMEENGINE_GJK_H
