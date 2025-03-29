//
// Created by eceva on 3/25/2025.
//

#include <vector>
#include "../rendering/partitioning/bvh-bounding-volume.h"
#include "../rendering/partitioning/bvh-node.h"
#include "Camera.h"
#include "../rendering/Ray.h"

#ifndef GAMEENGINE_CULLING_H
#define GAMEENGINE_CULLING_H

#endif //GAMEENGINE_CULLING_H

namespace Culling {
    bool isRayInside(BVH::Node* node, Ray* ray) {
        /**
         * 6 Faces
         * Face1:
         *
         * */

    }

    std::vector<BVH::BoundingVolume> getBVWithinCamera(BVH::Node* root, Camera* camera) {

    }
}
