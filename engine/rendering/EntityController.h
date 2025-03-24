//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_ENTITYCONTROLLER_H
#define GAMEENGINE_ENTITYCONTROLLER_H

#include <vector>
#include "Entity.h"
#include "Renderer.h"
#include "memory"
#include "../physics/RigidBody.h"
#include "../core/partitioning/bvh-node.h"

class EntityController : Renderer {
    BVH::Node* bvhRoot = nullptr;
public:
    std::vector<Entity*> entities;

    int update(float deltaTime) override;
    void render(glm::mat4 view, glm::mat4 projection) override;
    void addEntity(Entity* entity);
    void debugRender(glm::mat4 view, glm::mat4 projection);

    void buildBVH();
};

#endif //GAMEENGINE_ENTITYCONTROLLER_H
