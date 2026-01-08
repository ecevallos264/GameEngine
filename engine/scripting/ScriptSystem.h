#ifndef GAMEENGINE_SCRIPT_SYSTEM_H
#define GAMEENGINE_SCRIPT_SYSTEM_H

#include "../runtime/ISystem.h"
#include "../ecs/Registry.h"
#include "../ecs/components/TransformComponent.h"
#include "../rendering/SceneController.h"
#include "ScriptComponent.h"
#include "LuaBindings.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <unordered_map>
#include <memory>
#include <iostream>
#include <filesystem>

namespace Scripting {

// Script instance that holds Lua state for one entity's script
struct ScriptInstance {
    uint64_t entityId;
    std::string scriptPath;
    sol::state lua;
    ECS::Registry* registry = nullptr;  // Store registry reference for bindings

    // Cached function references
    sol::protected_function onStart;
    sol::protected_function onUpdate;
    sol::protected_function onDestroy;

    bool hasOnStart = false;
    bool hasOnUpdate = false;
    bool hasOnDestroy = false;
};

class ScriptSystem : public ISystem {
public:
    ScriptSystem() = default;

    void initialize(SystemContext& context) override {
        (void)context;
        std::cout << "[ScriptSystem] Initialized" << std::endl;
    }

    void update(SystemContext& context) override {
        float deltaTime = context.deltaTime;

        // Get registry from current scene
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene) return;

        ECS::Registry& registry = currentScene->getRegistry();

        // Process all entities with scripts
        registry.view<ECS::ScriptComponent>().each(
            [this, deltaTime, &registry](ECS::Entity entity, ECS::ScriptComponent& script) {
                if (!script.enabled) return;

                // Initialize script if needed
                if (!script.initialized) {
                    initializeScript(entity, script, registry);
                }

                // Call onUpdate
                if (script.initialized && script.instanceId >= 0) {
                    callOnUpdate(script.instanceId, entity.id, deltaTime);
                }
            }
        );
    }

    void shutdown(SystemContext& context) override {
        (void)context;

        // Call onDestroy for all scripts
        for (auto& [id, instance] : scriptInstances) {
            if (instance->hasOnDestroy) {
                callOnDestroy(id, instance->entityId);
            }
        }

        scriptInstances.clear();
        std::cout << "[ScriptSystem] Shutdown" << std::endl;
    }

    // Hot-reload a specific script
    void reloadScript(const std::string& scriptPath) {
        for (auto& [id, instance] : scriptInstances) {
            if (instance->scriptPath == scriptPath) {
                std::cout << "[ScriptSystem] Hot-reloading: " << scriptPath << std::endl;

                // Reload the script file
                auto result = instance->lua.safe_script_file(scriptPath, sol::script_pass_on_error);
                if (!result.valid()) {
                    sol::error err = result;
                    std::cerr << "[ScriptSystem] Reload error: " << err.what() << std::endl;
                    continue;
                }

                // Re-cache functions
                cacheScriptFunctions(*instance);

                // Re-run onStart after reload
                if (instance->hasOnStart) {
                    callOnStart(id, instance->entityId);
                }
            }
        }
    }

    // Get script count for debugging
    size_t getScriptCount() const { return scriptInstances.size(); }

private:
    std::unordered_map<int, std::unique_ptr<ScriptInstance>> scriptInstances;
    int nextInstanceId = 0;

    void initializeScript(ECS::Entity entity, ECS::ScriptComponent& script, ECS::Registry& registry) {
        // Check if script file exists
        if (!std::filesystem::exists(script.scriptPath)) {
            std::cerr << "[ScriptSystem] Script not found: " << script.scriptPath << std::endl;
            script.enabled = false;
            return;
        }

        auto instance = std::make_unique<ScriptInstance>();
        instance->entityId = entity.id;
        instance->scriptPath = script.scriptPath;

        // Open standard Lua libraries
        instance->lua.open_libraries(
            sol::lib::base,
            sol::lib::math,
            sol::lib::string,
            sol::lib::table
        );

        // Register engine bindings
        registerBindings(instance->lua, registry);

        // Load and execute the script
        auto result = instance->lua.safe_script_file(script.scriptPath, sol::script_pass_on_error);
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "[ScriptSystem] Failed to load script: " << script.scriptPath << std::endl;
            std::cerr << "[ScriptSystem] Error: " << err.what() << std::endl;
            script.enabled = false;
            return;
        }

        // Cache function references
        cacheScriptFunctions(*instance);

        // Store instance
        int instanceId = nextInstanceId++;
        script.instanceId = instanceId;

        // Call onStart
        if (instance->hasOnStart) {
            auto startResult = instance->onStart(entity.id);
            if (!startResult.valid()) {
                sol::error err = startResult;
                std::cerr << "[Script:" << instanceId << "] onStart error: " << err.what() << std::endl;
            }
        }

        script.initialized = true;
        scriptInstances[instanceId] = std::move(instance);

        std::cout << "[ScriptSystem] Loaded: " << script.scriptPath << " (instance " << instanceId << ")" << std::endl;
    }

    void cacheScriptFunctions(ScriptInstance& instance) {
        // Cache onStart
        sol::object startObj = instance.lua["onStart"];
        if (startObj.is<sol::protected_function>()) {
            instance.onStart = startObj.as<sol::protected_function>();
            instance.hasOnStart = true;
        } else {
            instance.hasOnStart = false;
        }

        // Cache onUpdate
        sol::object updateObj = instance.lua["onUpdate"];
        if (updateObj.is<sol::protected_function>()) {
            instance.onUpdate = updateObj.as<sol::protected_function>();
            instance.hasOnUpdate = true;
        } else {
            instance.hasOnUpdate = false;
        }

        // Cache onDestroy
        sol::object destroyObj = instance.lua["onDestroy"];
        if (destroyObj.is<sol::protected_function>()) {
            instance.onDestroy = destroyObj.as<sol::protected_function>();
            instance.hasOnDestroy = true;
        } else {
            instance.hasOnDestroy = false;
        }
    }

    void callOnStart(int instanceId, uint64_t entityId) {
        auto it = scriptInstances.find(instanceId);
        if (it == scriptInstances.end() || !it->second->hasOnStart) return;

        auto result = it->second->onStart(entityId);
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "[Script:" << instanceId << "] onStart error: " << err.what() << std::endl;
        }
    }

    void callOnUpdate(int instanceId, uint64_t entityId, float deltaTime) {
        auto it = scriptInstances.find(instanceId);
        if (it == scriptInstances.end() || !it->second->hasOnUpdate) return;

        auto result = it->second->onUpdate(entityId, deltaTime);
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "[Script:" << instanceId << "] onUpdate error: " << err.what() << std::endl;
        }
    }

    void callOnDestroy(int instanceId, uint64_t entityId) {
        auto it = scriptInstances.find(instanceId);
        if (it == scriptInstances.end() || !it->second->hasOnDestroy) return;

        auto result = it->second->onDestroy(entityId);
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "[Script:" << instanceId << "] onDestroy error: " << err.what() << std::endl;
        }
    }
};

} // namespace Scripting

#endif //GAMEENGINE_SCRIPT_SYSTEM_H
