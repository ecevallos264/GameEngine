#ifndef GAMEENGINE_ECS_ENTITY_H
#define GAMEENGINE_ECS_ENTITY_H

#include <cstdint>
#include <limits>

namespace ECS {

using EntityID = uint64_t;
constexpr EntityID INVALID_ENTITY = std::numeric_limits<EntityID>::max();

struct Entity {
    EntityID id = INVALID_ENTITY;

    Entity() = default;
    explicit Entity(EntityID id) : id(id) {}

    bool isValid() const { return id != INVALID_ENTITY; }

    bool operator==(const Entity& other) const { return id == other.id; }
    bool operator!=(const Entity& other) const { return id != other.id; }
    bool operator<(const Entity& other) const { return id < other.id; }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_ENTITY_H
