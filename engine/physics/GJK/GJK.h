//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_GJK_H
#define GAMEENGINE_GJK_H

#include "glm/vec3.hpp"
#include "../Simplex.h"
#include "../../entitities/shape.h"

glm::vec3 minkowskiSupport(std::shared_ptr<Shape> shapeA, std::shared_ptr<Shape> shapeB, const glm::vec3& direction) {
    glm::vec3 supportA = shapeA->getSupportPoint(direction);
    glm::vec3 supportB = shapeB->getSupportPoint(-direction);
    return supportA - supportB;
}



bool handleLine(Simplex& simplex, glm::vec3& direction) {
    glm::vec3 A = simplex.getLastPoint();
    glm::vec3 B = simplex.points[0];
    glm::vec3 AB = B - A;
    glm::vec3 AO = -A;

    // New direction is perpendicular to AB, toward the origin
    direction = glm::cross(glm::cross(AB, AO), AB);
    return false;
}

bool handleTriangle(Simplex& simplex, glm::vec3& direction) {
    glm::vec3 A = simplex.getLastPoint();
    glm::vec3 B = simplex.points[1];
    glm::vec3 C = simplex.points[0];

    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    glm::vec3 AO = -A;

    glm::vec3 ABC = glm::cross(AB, AC);

    if (glm::dot(glm::cross(ABC, AC), AO) > 0) {
        simplex.removePoint(1);
        direction = glm::cross(glm::cross(AC, AO), AC);
    } else if (glm::dot(glm::cross(AB, ABC), AO) > 0) {
        simplex.removePoint(0);
        direction = glm::cross(glm::cross(AB, AO), AB);
    } else {
        direction = ABC; // Toward the origin
    }

    return false;
}

bool handleTetrahedron(Simplex& simplex) {
    // If the origin is inside the tetrahedron, a collision is detected
    return true;
}

bool processSimplex(Simplex& simplex, glm::vec3& direction) {
    switch (simplex.size()) {
        case 2: // Line segment
            return handleLine(simplex, direction);
        case 3: // Triangle
            return handleTriangle(simplex, direction);
        case 4: // Tetrahedron
            return handleTetrahedron(simplex);
        default:
            return false;
    }
}

bool GJK(std::shared_ptr<Shape> shapeA, std::shared_ptr<Shape> shapeB) {
    // Initial direction
    glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);

    // Create the simplex
    Simplex simplex;

    // Add the first point to the simplex
    simplex.addPoint(minkowskiSupport(shapeA, shapeB, direction));

    // Reverse the direction toward the origin
    direction = -simplex.getLastPoint();

    while (true) {
        // Add a new point in the current direction
        glm::vec3 newPoint = minkowskiSupport(shapeA, shapeB, direction);

        // If the new point isn't past the origin in the current direction, no collision
        if (glm::dot(newPoint, direction) <= 0) {
            return false;
        }

        // Add the new point to the simplex
        simplex.addPoint(newPoint);

        // Process the simplex and determine the next direction
        if (processSimplex(simplex, direction)) {
            return true; // Collision detected
        }
    }
}


#endif //GAMEENGINE_GJK_H
