//
// Created by eceva on 3/25/2025.
//

#ifndef GAMEENGINE_PLANE_H
#define GAMEENGINE_PLANE_H

#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include "../../rendering/Ray.h"

struct Plane {
    glm::vec3 normal;
    float distance;

    Plane() : normal(glm::vec3()), distance(0.0f) {}

    Plane(const glm::vec3& n, float d) : normal(n), distance(d) {}

    float distanceToPoint(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }

    float findRayIntersectionScalar(Ray ray) const {
        float denominator = glm::dot(normal, ray.getDirection());

        if (fabs(denominator) < 1e-6) {
            return -1.0f;
        }

        float t = -((glm::dot(normal, ray.getOrigin()) + distance) / denominator);
        return t;
    }

    bool intersectsWithRay(Ray ray) {
        float t = findRayIntersectionScalar(ray);

        if (t < 0.0f) {
            return false;
        }
        return true;
    }
};

#endif //GAMEENGINE_PLANE_H
