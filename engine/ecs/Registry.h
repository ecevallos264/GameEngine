#ifndef GAMEENGINE_ECS_REGISTRY_H
#define GAMEENGINE_ECS_REGISTRY_H

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <set>
#include <functional>
#include "Entity.h"
#include "Component.h"
#include "ComponentPool.h"

namespace ECS {

class Registry {
public:
    // Create a new entity
    Entity create() {
        EntityID id = nextEntityID++;
        entities.insert(id);
        return Entity(id);
    }

    // Destroy an entity and all its components
    void destroy(Entity entity) {
        if (!isValid(entity)) return;

        // Remove all components for this entity
        for (auto& [type, pool] : componentPools) {
            pool->remove(entity.id);
        }

        entities.erase(entity.id);
    }

    // Check if entity is valid
    bool isValid(Entity entity) const {
        return entities.find(entity.id) != entities.end();
    }

    // Add a component to an entity
    template<typename T, typename... Args>
    T& emplace(Entity entity, Args&&... args) {
        auto& pool = getOrCreatePool<T>();
        return pool.emplace(entity.id, std::forward<Args>(args)...);
    }

    // Add a component to an entity (copy version)
    template<typename T>
    T& add(Entity entity, T component) {
        auto& pool = getOrCreatePool<T>();
        return pool.add(entity.id, std::move(component));
    }

    // Get a component from an entity
    template<typename T>
    T* get(Entity entity) {
        auto* pool = getPool<T>();
        if (!pool) return nullptr;
        return pool->get(entity.id);
    }

    template<typename T>
    const T* get(Entity entity) const {
        auto* pool = getPool<T>();
        if (!pool) return nullptr;
        return pool->get(entity.id);
    }

    // Check if entity has a component
    template<typename T>
    bool has(Entity entity) const {
        auto* pool = getPool<T>();
        if (!pool) return false;
        return pool->has(entity.id);
    }

    // Remove a component from an entity
    template<typename T>
    void remove(Entity entity) {
        auto* pool = getPool<T>();
        if (pool) pool->remove(entity.id);
    }

    // Get the component pool for a type
    template<typename T>
    ComponentPool<T>* getPool() {
        std::type_index type = typeid(T);
        auto it = componentPools.find(type);
        if (it == componentPools.end()) return nullptr;
        return static_cast<ComponentPool<T>*>(it->second.get());
    }

    template<typename T>
    const ComponentPool<T>* getPool() const {
        std::type_index type = typeid(T);
        auto it = componentPools.find(type);
        if (it == componentPools.end()) return nullptr;
        return static_cast<const ComponentPool<T>*>(it->second.get());
    }

    // View for iterating entities with specific components
    template<typename... Components>
    class View {
    public:
        View(Registry& registry) : registry(registry) {}

        class Iterator {
        public:
            Iterator(Registry& registry, std::set<EntityID>::iterator it, std::set<EntityID>::iterator end)
                : registry(registry), current(it), end(end) {
                advanceToValid();
            }

            bool operator!=(const Iterator& other) const { return current != other.current; }

            void operator++() {
                ++current;
                advanceToValid();
            }

            Entity operator*() const {
                return Entity(*current);
            }

        private:
            void advanceToValid() {
                while (current != end && !hasAllComponents()) {
                    ++current;
                }
            }

            bool hasAllComponents() const {
                return (registry.has<Components>(Entity(*current)) && ...);
            }

            Registry& registry;
            std::set<EntityID>::iterator current;
            std::set<EntityID>::iterator end;
        };

        Iterator begin() { return Iterator(registry, registry.entities.begin(), registry.entities.end()); }
        Iterator end() { return Iterator(registry, registry.entities.end(), registry.entities.end()); }

        // Iterate with callback
        void each(std::function<void(Entity, Components&...)> func) {
            for (Entity entity : *this) {
                func(entity, *registry.get<Components>(entity)...);
            }
        }

    private:
        Registry& registry;
    };

    // Get a view of entities with specific components
    template<typename... Components>
    View<Components...> view() {
        return View<Components...>(*this);
    }

    // Get all entities
    const std::set<EntityID>& getEntities() const { return entities; }

    size_t entityCount() const { return entities.size(); }

private:
    template<typename T>
    ComponentPool<T>& getOrCreatePool() {
        std::type_index type = typeid(T);
        auto it = componentPools.find(type);
        if (it == componentPools.end()) {
            auto pool = std::make_unique<ComponentPool<T>>();
            auto& ref = *pool;
            componentPools[type] = std::move(pool);
            return ref;
        }
        return *static_cast<ComponentPool<T>*>(it->second.get());
    }

    std::set<EntityID> entities;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools;
    EntityID nextEntityID = 0;
};

} // namespace ECS

#endif //GAMEENGINE_ECS_REGISTRY_H
