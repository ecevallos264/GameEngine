//
// Created by eceva on 2/23/2025.
//

#ifndef UPDATECONTROLLER_H
#define UPDATECONTROLLER_H
#include "Entity.h"

class UpdateController {
    std::vector<Entity*> STATIC_ENTITIES;
    std::vector<Entity*> DYNAMIC_ENTITIES;

public:
    UpdateController() = default;
    ~UpdateController() = default;

    void addEntity(Entity* entity) {
        if (entity->isFixed()) {
            STATIC_ENTITIES.push_back(entity);
        } else {
            DYNAMIC_ENTITIES.push_back(entity);
        }
    }

    int update(double deltaTime) {
        for (auto entity : DYNAMIC_ENTITIES) {
            entity->update(deltaTime);
        }
    }

};

#endif //UPDATECONTROLLER_H
