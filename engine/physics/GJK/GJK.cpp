// Created by eceva on 1/16/2025.
#include "GJK.h"
#include "glm/vec3.hpp"
#include "../../core/settings/settings.h"
#include <memory>

// Computes the Minkowski difference's support point
glm::vec3 minkowskiSupport(Entity* shapeA, Entity* shapeB, const glm::vec3& direction) {
    glm::vec3 supportA = shapeA->getSupportPoint(direction);
    glm::vec3 supportB = shapeB->getSupportPoint(-direction);
    return supportA - supportB;
}

// GJK collision detection function
CollisionState GJK(Entity* shapeA, Entity* shapeB) {
    glm::vec3 direction = shapeB->position - shapeA->position;
    if (glm::length(direction) == 0.0f) {
        direction = glm::vec3(1.0f, 0.0f, 0.0f); // Default direction
    }

    Simplex simplex;
    simplex.addPoint(minkowskiSupport(shapeA, shapeB, direction));

    direction = -simplex.getLastPoint();
    int iteration = 0;
    while (iteration++ < Settings::MAX_GJK_ITERATION) {
        glm::vec3 newPoint = minkowskiSupport(shapeA, shapeB, direction);

        // If the new point doesn't pass the origin in the current direction, no collision
        if (glm::dot(newPoint, direction) <= 0) {
            return CollisionState::NO_COLLISION;
        }

        simplex.addPoint(newPoint);

        // Check if the simplex contains the origin and update direction
        if (simplex.containsOrigin(direction)) {
            return CollisionState::COLLISION;
        }

        // Ensure the direction is valid (non-zero)
        if (glm::length(direction) < 1e-6f) {
            return CollisionState::NO_COLLISION; // Failsafe for degenerate cases
        }
    }

}

bool check(Entity* shapeA, Entity* shapeB) {
    CollisionState result = GJK(shapeA, shapeB);
    return result == CollisionState::COLLISION;

}
