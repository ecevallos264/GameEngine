#ifndef GAMEENGINE_SYSTEM_MANAGER_H
#define GAMEENGINE_SYSTEM_MANAGER_H

#include <memory>
#include <vector>
#include "ISystem.h"

class SystemManager {
public:
    template <typename T, typename... Args>
    T& addSystem(Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *system;
        systems.emplace_back(std::move(system));
        return ref;
    }

    void initializeAll(SystemContext& context);
    void shutdownAll(SystemContext& context);

    void updateAll(SystemContext& context);
    void fixedUpdateAll(SystemContext& context);
    void renderAll(SystemContext& context);

private:
    std::vector<std::unique_ptr<ISystem>> systems;
};

#endif //GAMEENGINE_SYSTEM_MANAGER_H
