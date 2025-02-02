//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_COLLISIONHANDLER_H
#define GAMEENGINE_COLLISIONHANDLER_H

#include "../core/patterns/Singleton.h"
#include "../rendering/Entity.h"
#include "../entitities/Shape.h"
#include "GJK/GJK.h"
#include "CollisionEvent.h"
#include "../../test_game/entities/MyCube.h"

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

class CollisionHandler : public Singleton<CollisionHandler>, EventListener {
public:
    CollisionHandler() : Singleton<CollisionHandler>() {

    }

    bool checkForCollision(Shape* entityA, Shape* entityB) {
        return check(entityA, entityB);
    }

    void handleCollision(Entity* entity1, Entity* entity2) {
        std::cout << "Generic Collision Handling" << std::endl;
    }


//    template<typename T>
//    void subscribe(std::type_index index, std::function<void(CollisionData)> callback) {
//        CollisionEntry entry = (T::getType() < index)
//                               ? CollisionEntry{T::getType(), index}
//                               : CollisionEntry{index, T::getType()};
//        registry[entry] = callback;
//    }
//
//    void onEvent(CollisionEvent event) {
//
//        CollisionEntry entry = (std::type_index(typeid(*event.entityA)) < std::type_index(typeid(*event.entityB)))
//                               ? CollisionEntry{std::type_index(typeid(*event.entityA)), std::type_index(typeid(*event.entityB))}
//                               : CollisionEntry{std::type_index(typeid(*event.entityB)), std::type_index(typeid(*event.entityA))};
//
//        auto it = registry.find(entry);
//        if (it != registry.end()) {
//            it->second({event.entityA, event.entityB});
//        } else {
//            std::cout << "No collision callback registered for this event." << std::endl;
//        }
//    }

    void onEvent(const Event& event) override {

    }
};

#endif //GAMEENGINE_COLLISIONHANDLER_H
