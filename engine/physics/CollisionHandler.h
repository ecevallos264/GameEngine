//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_COLLISIONHANDLER_H
#define GAMEENGINE_COLLISIONHANDLER_H

#include "../core/patterns/Singleton.h"
#include "../rendering/Entity.h"
#include "../entitities/shape.h"
#include "GJK/GJK.h"
#include "CollisionEvent.h"

struct CollisionEntry {
    std::type_index entityA;
    std::type_index entityB;
    bool operator==(const CollisionEntry& other) const {
        return entityA == other.entityA && entityB == other.entityB;
    }
};

namespace std {
    template <>
    struct hash<CollisionEntry> {
        std::size_t operator()(const CollisionEntry& entry) const {
            return hash<std::type_index>()(entry.entityA) ^ hash<std::type_index>()(entry.entityB);
        }
    };
}

struct CollisionData {
    Entity* entityA;
    Entity* entityB;


};

class CollisionHandler : public Singleton<CollisionHandler> {
private:
    std::unordered_map<CollisionEntry, std::function<void(CollisionData)>> registry;
public:
    CollisionHandler() : Singleton<CollisionHandler>() {
    }

    bool handleCollision(Entity* entityA, Entity* entityB) {
        return check(entityA, entityB);
    }

    template<typename T>
    void subscribe(std::type_index index, std::function<void(CollisionData)> callback) {
        CollisionEntry entry = (T::getType() < index)
                               ? CollisionEntry{T::getType(), index}
                               : CollisionEntry{index, T::getType()};
        registry[entry] = callback;
    }

    void onEvent(CollisionEvent event) {
        CollisionEntry entry = {typeid(*event.entityA), typeid(*event.entityB)};
        auto it = registry.find(entry);
        if (it != registry.end()) {
            it->second({event.entityA, event.entityB});
        }
    }
};

#endif //GAMEENGINE_COLLISIONHANDLER_H
