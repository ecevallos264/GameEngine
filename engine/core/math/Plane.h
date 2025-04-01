#ifndef GAMEENGINE_PLANE_H
#define GAMEENGINE_PLANE_H

#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

class Plane {
public:
    glm::vec3 normal;
    float distance;

    Plane() : normal(glm::vec3(0.0f)), distance(0.0f) {}
    Plane(const glm::vec3& normal, float distance) : normal(glm::normalize(normal)), distance(distance) {}

    static Plane fromPointNormal(const glm::vec3& point, const glm::vec3& normal) {
        float distance = -glm::dot(normal, point);
        return Plane(normal, distance);
    }

    float distanceToPoint(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }

    bool isInFront(const glm::vec3& point) const {
        return distanceToPoint(point) > 0;
    }

    bool isBehind(const glm::vec3& point) const {
        return distanceToPoint(point) < 0;
    }
};

#endif
