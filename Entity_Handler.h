//
// Created by eceva on 11/19/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_ENTITY_HANDLER_H
#define SOFTWAREENGINEERINGPROJECT_ENTITY_HANDLER_H

#include <vector>
#include "Singleton.h"
#include "shape.h"

class EntityHandler : public Singleton<EntityHandler> {
public:
    EntityHandler() = default;
    std::vector<Shape*> entities;
    int getEntitySize() {
        return this->entities.size();
    }

    std::vector<Shape*> getEntities() {
        return entities;
    }

    void addEntity(Shape* shape) {
        this->entities.push_back(shape);
        shape->initializeBuffers();
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_ENTITY_HANDLER_H
