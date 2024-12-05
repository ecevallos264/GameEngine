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

    void removeEntity(Shape* shape) {
        auto it = std::find(entities.begin(), entities.end(), shape);
        if (it != entities.end()) {
            entities.erase(it);
        }
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_ENTITY_HANDLER_H
