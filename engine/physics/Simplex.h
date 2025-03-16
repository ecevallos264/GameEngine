// Created by eceva on 1/15/2025.

#ifndef GAMEENGINE_SIMPLEX_H
#define GAMEENGINE_SIMPLEX_H

#include <memory>
#include "glm/vec3.hpp"
#include "RigidBody.h"

struct Simplex {
    std::vector<glm::vec3> points;

    void addPoint(const glm::vec3& point) {
        points.push_back(point);
    }

    glm::vec3 getLastPoint() const {
        return points.back();
    }

    void removePoint(size_t index) {
        points.erase(points.begin() + index);
    }

    size_t size() const {
        return points.size();
    }

    // Determines if the simplex contains the origin and updates the direction
    bool containsOrigin(glm::vec3& direction) {
        const glm::vec3& a = points.back();
        glm::vec3 ao = -a;

        if (points.size() == 3) {
            // Triangle case
            const glm::vec3& b = points[1];
            const glm::vec3& c = points[0];

            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 abc = glm::cross(ab, ac);

            // Check if the origin is in the region of the triangle
            if (glm::dot(glm::cross(abc, ac), ao) > 0) {
                // Remove b and adjust direction
                points.erase(points.begin() + 1);
                direction = glm::cross(glm::cross(ac, ao), ac);
                return false;
            } else if (glm::dot(glm::cross(ab, abc), ao) > 0) {
                // Remove c and adjust direction
                points.erase(points.begin());
                direction = glm::cross(glm::cross(ab, ao), ab);
                return false;
            } else {
                // Origin is within the triangle
                if (glm::dot(abc, ao) > 0) {
                    direction = abc;
                } else {
                    std::swap(points[0], points[1]);
                    direction = -abc;
                }
                return true;
            }
        } else if (points.size() == 2) {
            // Line segment case
            const glm::vec3& b = points[0];
            glm::vec3 ab = b - a;

            direction = glm::cross(glm::cross(ab, ao), ab);
            return false;
        } else {
            // Point case
            direction = ao;
            return false;
        }

        return false; // Fallback safety, shouldn't reach here
    }
};

#endif //GAMEENGINE_SIMPLEX_H
