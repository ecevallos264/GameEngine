//
// Created by eceva on 3/22/2025.
//

#ifndef GAMEENGINE_BVH_BOUNDING_VOLUME_H
#define GAMEENGINE_BVH_BOUNDING_VOLUME_H

#include "glm/vec3.hpp"
#include "../../physics/RigidBody.h"
#include "../../core/math/Plane.h"

namespace BVH {
    class BoundingVolume {
    private:
        RigidBody* body; // can be undefined if bounding region is manually set
        glm::vec3 min = glm::vec3(0);
        glm::vec3 max = glm::vec3(0);
    public:
        BoundingVolume() = default;
        BoundingVolume(glm::vec3 p1, glm::vec3 p2);
        BoundingVolume(RigidBody* body);

        glm::vec3 getMin() const;
        glm::vec3 getMax() const;
        void expandToFit(const BoundingVolume& other);
        bool intersects(const BoundingVolume& other) const;
        bool contains(const glm::vec3& point) const;
        float surfaceArea() const;
        glm::vec3 getCenter() const;
        Plane getFront();
        Plane getBack();
        Plane getTop();
        Plane getBottom();
        Plane getLeft();
        Plane getRight();

    };
}

#endif //GAMEENGINE_BVH_BOUNDING_VOLUME_H
