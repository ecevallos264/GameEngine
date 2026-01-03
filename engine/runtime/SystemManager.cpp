#include "SystemManager.h"

void SystemManager::initializeAll(SystemContext& context) {
    for (const auto& system : systems) {
        system->initialize(context);
    }
}

void SystemManager::shutdownAll(SystemContext& context) {
    for (const auto& system : systems) {
        system->shutdown(context);
    }
}

void SystemManager::updateAll(SystemContext& context) {
    for (const auto& system : systems) {
        system->update(context);
    }
}

void SystemManager::fixedUpdateAll(SystemContext& context) {
    for (const auto& system : systems) {
        system->fixedUpdate(context);
    }
}

void SystemManager::renderAll(SystemContext& context) {
    for (const auto& system : systems) {
        system->render(context);
    }
}
