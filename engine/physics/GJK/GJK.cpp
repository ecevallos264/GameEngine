// Created by eceva on 1/16/2025.
#include "GJK.h"
#include "glm/vec3.hpp"
#include "../../core/settings/settings.h"
#include <memory>

// Computes the Minkowski difference's support point
glm::vec3 minkowskiSupport(Shape* shapeA, Shape* shapeB, const glm::vec3& direction) {
    glm::vec3 supportA = shapeA->getSupportPoint(direction);
    glm::vec3 supportB = shapeB->getSupportPoint(-direction);
    return supportA - supportB;
}

// GJK collision detection function
CollisionState GJK(Shape* shapeA, Shape* shapeB) {
    glm::vec3 direction = shapeB->getPosition() - shapeA->getPosition();
    if (glm::length(direction) == 0.0f) {
        direction = glm::vec3(1.0f, 0.0f, 0.0f); // Default direction
    }

    glm::vec3 a, b, c, d;
    glm::vec3 search_dir = direction;

    // Get initial point for simplex
    c = minkowskiSupport(shapeA, shapeB, search_dir);
    search_dir = -c; // Search in direction of origin

    // Get second point for a line segment simplex
    b = minkowskiSupport(shapeA, shapeB, search_dir);
    if (glm::dot(b, search_dir) < 0) {
        return CollisionState::NO_COLLISION; // Cannot reach the origin
    }

    search_dir = glm::cross(glm::cross(c - b, -b), c - b);
    if (glm::length(search_dir) == 0.0f) {
        search_dir = glm::cross(c - b, glm::vec3(1.0f, 0.0f, 0.0f));
        if (glm::length(search_dir) == 0.0f) {
            search_dir = glm::cross(c - b, glm::vec3(0.0f, 0.0f, -1.0f));
        }
    }

    int simplex_dim = 2;

    for (int iterations = 0; iterations < Settings::MAX_GJK_ITERATION; ++iterations) {
        a = minkowskiSupport(shapeA, shapeB, search_dir);
        if (glm::dot(a, search_dir) < 0) {
            return CollisionState::NO_COLLISION; // Cannot reach the origin
        }

        ++simplex_dim;
        if (simplex_dim == 3) {
            update_simplex3(a, b, c, d, simplex_dim, search_dir);
        } else if (update_simplex4(a, b, c, d, simplex_dim, search_dir)) {
            return CollisionState::COLLISION;
        }
    }

    return CollisionState::NO_COLLISION; // Maximum iterations reached
}

// Triangle case
void update_simplex3(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, glm::vec3 &d, int &simp_dim, glm::vec3 &search_dir) {
    glm::vec3 n = glm::cross(b - a, c - a); // Triangle's normal
    glm::vec3 AO = -a; // Direction to origin

    simp_dim = 2;
    if (glm::dot(glm::cross(b - a, n), AO) > 0) { // Closest to edge AB
        c = a;
        search_dir = glm::cross(glm::cross(b - a, AO), b - a);
        return;
    }
    if (glm::dot(glm::cross(n, c - a), AO) > 0) { // Closest to edge AC
        b = a;
        search_dir = glm::cross(glm::cross(c - a, AO), c - a);
        return;
    }

    simp_dim = 3;
    if (glm::dot(n, AO) > 0) { // Above triangle
        d = c;
        c = b;
        b = a;
        search_dir = n;
        return;
    }
    d = b;
    b = a;
    search_dir = -n;
}

// Tetrahedral case
bool update_simplex4(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, glm::vec3 &d, int &simp_dim, glm::vec3 &search_dir) {
    glm::vec3 ABC = glm::cross(b - a, c - a);
    glm::vec3 ACD = glm::cross(c - a, d - a);
    glm::vec3 ADB = glm::cross(d - a, b - a);

    glm::vec3 AO = -a;
    simp_dim = 3;

    if (glm::dot(ABC, AO) > 0) {
        d = c;
        c = b;
        b = a;
        search_dir = ABC;
        return false;
    }
    if (glm::dot(ACD, AO) > 0) {
        b = a;
        search_dir = ACD;
        return false;
    }
    if (glm::dot(ADB, AO) > 0) {
        c = d;
        d = b;
        b = a;
        search_dir = ADB;
        return false;
    }

    return true; // Enclosed
}

// Function to check collision between two entities
bool check(Shape* shapeA, Shape* shapeB) {
    CollisionState result = GJK(shapeA, shapeB);
    return result == CollisionState::COLLISION;
}
