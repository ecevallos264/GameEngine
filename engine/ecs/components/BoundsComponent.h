#ifndef GAMEENGINE_ECS_BOUNDS_COMPONENT_H
#define GAMEENGINE_ECS_BOUNDS_COMPONENT_H

#include <glm/glm.hpp>
#include <vector>
#include "../../entities/Vertex.h"

namespace ECS {

struct BoundsComponent {
    // Local-space AABB (relative to entity origin)
    glm::vec3 localMin = glm::vec3(-0.5f);
    glm::vec3 localMax = glm::vec3(0.5f);

    // World-space AABB (computed by FrustumCullingSystem)
    glm::vec3 worldMin = glm::vec3(-0.5f);
    glm::vec3 worldMax = glm::vec3(0.5f);

    // Visibility flag set by frustum culling
    bool visible = true;

    // Dirty flag for recalculation
    bool dirty = true;

    BoundsComponent() = default;

    BoundsComponent(const glm::vec3& min, const glm::vec3& max)
        : localMin(min), localMax(max), worldMin(min), worldMax(max) {}

    // Calculate bounds from mesh vertices
    static BoundsComponent fromVertices(const std::vector<Vertex>& vertices) {
        if (vertices.empty()) return BoundsComponent();

        glm::vec3 min = vertices[0].position;
        glm::vec3 max = vertices[0].position;

        for (const auto& v : vertices) {
            min = glm::min(min, v.position);
            max = glm::max(max, v.position);
        }

        return BoundsComponent(min, max);
    }

    // Get center of local bounds
    glm::vec3 getLocalCenter() const {
        return (localMin + localMax) * 0.5f;
    }

    // Get extents (half-size) of local bounds
    glm::vec3 getLocalExtents() const {
        return (localMax - localMin) * 0.5f;
    }

    // Get center of world bounds
    glm::vec3 getWorldCenter() const {
        return (worldMin + worldMax) * 0.5f;
    }

    // Get extents of world bounds
    glm::vec3 getWorldExtents() const {
        return (worldMax - worldMin) * 0.5f;
    }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_BOUNDS_COMPONENT_H
