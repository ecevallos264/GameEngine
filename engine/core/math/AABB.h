#ifndef GAMEENGINE_AABB_H
#define GAMEENGINE_AABB_H

#include <glm/glm.hpp>
#include <algorithm>
#include <limits>

struct AABB {
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());

    AABB() = default;

    AABB(const glm::vec3& min, const glm::vec3& max)
        : min(min), max(max) {}

    // Check if AABB is valid (has been set)
    bool isValid() const {
        return min.x <= max.x && min.y <= max.y && min.z <= max.z;
    }

    // Get center of the AABB
    glm::vec3 getCenter() const {
        return (min + max) * 0.5f;
    }

    // Get half-extents
    glm::vec3 getExtents() const {
        return (max - min) * 0.5f;
    }

    // Get size
    glm::vec3 getSize() const {
        return max - min;
    }

    // Get surface area (useful for SAH)
    float getSurfaceArea() const {
        glm::vec3 d = max - min;
        return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
    }

    // Get volume
    float getVolume() const {
        glm::vec3 d = max - min;
        return d.x * d.y * d.z;
    }

    // Get the longest axis (0=x, 1=y, 2=z)
    int getLongestAxis() const {
        glm::vec3 d = max - min;
        if (d.x > d.y && d.x > d.z) return 0;
        if (d.y > d.z) return 1;
        return 2;
    }

    // Expand to include a point
    void expand(const glm::vec3& point) {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }

    // Expand to include another AABB
    void expand(const AABB& other) {
        min = glm::min(min, other.min);
        max = glm::max(max, other.max);
    }

    // Check intersection with another AABB
    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    // Check if contains a point
    bool contains(const glm::vec3& point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }

    // Check if fully contains another AABB
    bool contains(const AABB& other) const {
        return (other.min.x >= min.x && other.max.x <= max.x) &&
               (other.min.y >= min.y && other.max.y <= max.y) &&
               (other.min.z >= min.z && other.max.z <= max.z);
    }

    // Merge two AABBs
    static AABB merge(const AABB& a, const AABB& b) {
        return AABB(
            glm::min(a.min, b.min),
            glm::max(a.max, b.max)
        );
    }

    // Transform AABB by a matrix (returns new AABB that bounds the transformed box)
    AABB transform(const glm::mat4& matrix) const {
        // Get all 8 corners
        glm::vec3 corners[8] = {
            glm::vec3(min.x, min.y, min.z),
            glm::vec3(max.x, min.y, min.z),
            glm::vec3(min.x, max.y, min.z),
            glm::vec3(max.x, max.y, min.z),
            glm::vec3(min.x, min.y, max.z),
            glm::vec3(max.x, min.y, max.z),
            glm::vec3(min.x, max.y, max.z),
            glm::vec3(max.x, max.y, max.z)
        };

        AABB result;
        for (int i = 0; i < 8; ++i) {
            glm::vec4 transformed = matrix * glm::vec4(corners[i], 1.0f);
            result.expand(glm::vec3(transformed));
        }
        return result;
    }
};

#endif //GAMEENGINE_AABB_H
