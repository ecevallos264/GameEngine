#ifndef GAMEENGINE_SCRIPT_COMPONENT_H
#define GAMEENGINE_SCRIPT_COMPONENT_H

#include <string>
#include <unordered_map>
#include <variant>

namespace ECS {

// Script variable types that can be exposed to Lua
using ScriptValue = std::variant<bool, int, float, double, std::string>;

struct ScriptComponent {
    // Path to the Lua script file
    std::string scriptPath;

    // Script instance state (is it initialized?)
    bool initialized = false;

    // Script enabled state
    bool enabled = true;

    // Exposed properties that can be set in editor and accessed in Lua
    std::unordered_map<std::string, ScriptValue> properties;

    // Unique instance ID for this script (used by ScriptSystem)
    int instanceId = -1;

    ScriptComponent() = default;
    explicit ScriptComponent(const std::string& path) : scriptPath(path) {}

    // Helper to set properties
    template<typename T>
    void set(const std::string& name, T value) {
        properties[name] = value;
    }

    // Helper to get properties
    template<typename T>
    T get(const std::string& name, T defaultValue = T{}) const {
        auto it = properties.find(name);
        if (it != properties.end()) {
            if (auto* val = std::get_if<T>(&it->second)) {
                return *val;
            }
        }
        return defaultValue;
    }
};

} // namespace ECS

#endif //GAMEENGINE_SCRIPT_COMPONENT_H
