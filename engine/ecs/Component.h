#ifndef GAMEENGINE_ECS_COMPONENT_H
#define GAMEENGINE_ECS_COMPONENT_H

#include <cstdint>
#include <typeindex>
#include <atomic>

namespace ECS {

using ComponentTypeID = std::size_t;

class ComponentTypeRegistry {
public:
    template<typename T>
    static ComponentTypeID getTypeID() {
        static ComponentTypeID id = nextID++;
        return id;
    }

private:
    static inline std::atomic<ComponentTypeID> nextID{0};
};

// Base component marker - components don't need to inherit from this,
// but it provides a common interface if needed
struct IComponent {
    virtual ~IComponent() = default;
};

} // namespace ECS

#endif //GAMEENGINE_ECS_COMPONENT_H
