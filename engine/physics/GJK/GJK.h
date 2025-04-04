//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_GJK_H
#define GAMEENGINE_GJK_H

#include <memory>
#include "glm/vec3.hpp"
#include "../Simplex.h"
#include "../RigidBody.h"

enum CollisionState {
    EVOLVING,
    COLLISION,
    NO_COLLISION
};

extern glm::vec3 minkowskiSupport(RigidBody* shapeA, RigidBody* shapeB, const glm::vec3& direction);
void update_simplex3(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, glm::vec3 &d, int &simp_dim, glm::vec3 &search_dir);
bool update_simplex4(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, glm::vec3 &d, int &simp_dim, glm::vec3 &search_dir);

extern CollisionState GJK(RigidBody* shapeA, RigidBody* shapeB);

extern bool check(RigidBody* shapeA, RigidBody* shapeB);



#endif //GAMEENGINE_GJK_H
