//
// Created by eceva on 3/22/2025.
//
#include "bvh-bounding-volume.h"
#include <algorithm>

namespace BVH {

    BoundingVolume::BoundingVolume(glm::vec3 p1, glm::vec3 p2) {
        min = glm::min(p1, p2);
        max = glm::max(p1, p2);
        body = nullptr;
    }

    BoundingVolume::BoundingVolume(RigidBody* body) : body(body) {
        if (!body) {
            std::cerr << "Error: RigidBody is nullptr in BoundingVolume constructor." << std::endl;
            min = glm::vec3(0.0f);
            max = glm::vec3(0.0f);
            return;
        }

        min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        const auto& vertices = body->getTransformedVertices();
        if (vertices.empty()) {
            std::cerr << "Error: RigidBody has no vertices in BoundingVolume constructor." << std::endl;
            min = glm::vec3(0.0f);
            max = glm::vec3(0.0f);
            return;
        }

        for (const Vertex& vertex : vertices) {
            min = glm::min(min, vertex.position);
            max = glm::max(max, vertex.position);
        }

        if (min == glm::vec3(FLT_MAX) || max == glm::vec3(-FLT_MAX)) {
            std::cerr << "Error: BoundingVolume could not calculate valid bounds." << std::endl;
            min = glm::vec3(0.0f);
            max = glm::vec3(0.0f);
        }
    }

    void BoundingVolume::expandToFit(const BoundingVolume& other) {
        min = glm::min(min, other.min);
        max = glm::max(max, other.max);
    }

    bool BoundingVolume::intersects(const BoundingVolume& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    bool BoundingVolume::contains(const glm::vec3& point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }

    float BoundingVolume::surfaceArea() const {
        glm::vec3 size = max - min;
        return 2.0f * (size.x * size.y + size.x * size.z + size.y * size.z);
    }

    glm::vec3 BoundingVolume::getCenter() const {
        return (min + max) * 0.5f;
    }

    glm::vec3 BoundingVolume::getMin() const {
        return min;
    }

    glm::vec3 BoundingVolume::getMax() const {
        return max;
    }

}
