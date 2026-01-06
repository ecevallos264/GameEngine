#ifndef GAMEENGINE_ECS_COMPONENT_POOL_H
#define GAMEENGINE_ECS_COMPONENT_POOL_H

#include <unordered_map>
#include <vector>
#include <memory>
#include "Entity.h"

namespace ECS {

// Type-erased base for component storage
class IComponentPool {
public:
    virtual ~IComponentPool() = default;
    virtual void remove(EntityID entity) = 0;
    virtual bool has(EntityID entity) const = 0;
};

// Typed component pool for storing components of type T
template<typename T>
class ComponentPool : public IComponentPool {
public:
    T& add(EntityID entity, T component) {
        auto it = entityToIndex.find(entity);
        if (it != entityToIndex.end()) {
            // Component already exists, update it
            components[it->second] = std::move(component);
            return components[it->second];
        }

        // Add new component
        size_t index = components.size();
        components.push_back(std::move(component));
        entityToIndex[entity] = index;
        indexToEntity.push_back(entity);
        return components.back();
    }

    template<typename... Args>
    T& emplace(EntityID entity, Args&&... args) {
        return add(entity, T{std::forward<Args>(args)...});
    }

    void remove(EntityID entity) override {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return;

        size_t indexToRemove = it->second;
        size_t lastIndex = components.size() - 1;

        if (indexToRemove != lastIndex) {
            // Swap with last element
            components[indexToRemove] = std::move(components[lastIndex]);
            EntityID movedEntity = indexToEntity[lastIndex];
            entityToIndex[movedEntity] = indexToRemove;
            indexToEntity[indexToRemove] = movedEntity;
        }

        components.pop_back();
        indexToEntity.pop_back();
        entityToIndex.erase(entity);
    }

    bool has(EntityID entity) const override {
        return entityToIndex.find(entity) != entityToIndex.end();
    }

    T* get(EntityID entity) {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return nullptr;
        return &components[it->second];
    }

    const T* get(EntityID entity) const {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return nullptr;
        return &components[it->second];
    }

    // Iteration support
    size_t size() const { return components.size(); }

    struct Iterator {
        ComponentPool& pool;
        size_t index;

        bool operator!=(const Iterator& other) const { return index != other.index; }
        void operator++() { ++index; }

        std::pair<EntityID, T&> operator*() {
            return {pool.indexToEntity[index], pool.components[index]};
        }
    };

    Iterator begin() { return {*this, 0}; }
    Iterator end() { return {*this, components.size()}; }

    // Access underlying data for systems
    const std::vector<EntityID>& getEntities() const { return indexToEntity; }
    std::vector<T>& getComponents() { return components; }
    const std::vector<T>& getComponents() const { return components; }

private:
    std::vector<T> components;
    std::unordered_map<EntityID, size_t> entityToIndex;
    std::vector<EntityID> indexToEntity;
};

} // namespace ECS

#endif //GAMEENGINE_ECS_COMPONENT_POOL_H
