//
// Created by eceva on 3/22/2025.
//

#ifndef GAMEENGINE_BVH_NODE_H
#define GAMEENGINE_BVH_NODE_H

#include "../../physics/RigidBody.h"
#include "bvh-bounding-volume.h"
#include "../shaders/ShaderManager.h"

namespace BVH {
    class Node {
    private:
        unsigned int VAO, VBO;
        bool isLeaf = true;
        Node* left = nullptr;
        Node* right = nullptr;
        BoundingVolume* boundingRegion = nullptr;

    public:
        Node() {}
        void insert(BoundingVolume* boundingVolume);
        void update();

        void cleanupWireframeBox();

        void render(glm::mat4 view, glm::mat4 projection, int level);

        void
        drawWireframeBox(const glm::vec3 &center, const glm::vec3 &size, glm::mat4 view, glm::mat4 projection,
                         glm::vec3 color);

        void setupWireframeBox(glm::vec3 color);

        void build(std::vector<BoundingVolume *> &volumes);
    };
}

#endif //GAMEENGINE_BVH_NODE_H
