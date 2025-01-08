#ifndef GAME_ENGINE_ENTITY_HANDLER_H
#define GAME_ENGINE_ENTITY_HANDLER_H

#include <vector>
#include "../utils/patterns/Singleton.h"
#include "shape.h"
#include "Entity.h"

class EntityHandler : public Singleton<EntityHandler> {
public:
    EntityHandler() = default;
    std::vector<Entity*> entities;
    int getEntitySize() {
        return this->entities.size();
    }

    std::vector<Entity*> getEntities() {
        return entities;
    }

    void addEntity(Entity* entity) {
        this->entities.push_back(entity);
    }

    void removeEntity(Entity* entity) {
        auto it = std::find(entities.begin(), entities.end(), entity);
        if (it != entities.end()) {
            entities.erase(it);
        }
    }

    static void updateEntities(float deltaTime) {
        for(Entity* entity: EntityHandler::getInstance().getEntities()) {
            entity->update(deltaTime, Game::getInstance().shader);
        }
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_ENTITY_HANDLER_H
