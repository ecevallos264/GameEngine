#ifndef GAMEENGINE_LUA_BINDINGS_H
#define GAMEENGINE_LUA_BINDINGS_H

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "../ecs/Registry.h"
#include "../ecs/Entity.h"
#include "../ecs/components/TransformComponent.h"
#include "../ecs/components/MeshComponent.h"
#include "../ecs/components/RenderComponent.h"
#include "../ecs/components/BoundsComponent.h"
#include "../io/IOSystem.h"
#include "../core/state/game_state.h"

#include <glm/glm.hpp>
#include <iostream>
#include <unordered_map>

namespace Scripting {

// Key name to key code mapping (returns -1 if not found)
inline int stringToKey(const std::string& keyName) {
    static const std::unordered_map<std::string, int> keyMap = {
        // Letters
        {"A", IO::Key::A}, {"B", IO::Key::B}, {"C", IO::Key::C}, {"D", IO::Key::D},
        {"E", IO::Key::E}, {"F", IO::Key::F}, {"G", IO::Key::G}, {"H", IO::Key::H},
        {"I", IO::Key::I}, {"J", IO::Key::J}, {"K", IO::Key::K}, {"L", IO::Key::L},
        {"M", IO::Key::M}, {"N", IO::Key::N}, {"O", IO::Key::O}, {"P", IO::Key::P},
        {"Q", IO::Key::Q}, {"R", IO::Key::R}, {"S", IO::Key::S}, {"T", IO::Key::T},
        {"U", IO::Key::U}, {"V", IO::Key::V}, {"W", IO::Key::W}, {"X", IO::Key::X},
        {"Y", IO::Key::Y}, {"Z", IO::Key::Z},

        // Numbers
        {"0", IO::Key::Num0}, {"1", IO::Key::Num1}, {"2", IO::Key::Num2},
        {"3", IO::Key::Num3}, {"4", IO::Key::Num4}, {"5", IO::Key::Num5},
        {"6", IO::Key::Num6}, {"7", IO::Key::Num7}, {"8", IO::Key::Num8},
        {"9", IO::Key::Num9},

        // Special keys
        {"Space", IO::Key::Space},
        {"Enter", IO::Key::Enter},
        {"Escape", IO::Key::Escape},
        {"Tab", IO::Key::Tab},
        {"Backspace", IO::Key::Backspace},
        {"LeftShift", IO::Key::LeftShift},
        {"RightShift", IO::Key::RightShift},
        {"LeftCtrl", IO::Key::LeftControl},
        {"RightCtrl", IO::Key::RightControl},
        {"LeftAlt", IO::Key::LeftAlt},
        {"RightAlt", IO::Key::RightAlt},

        // Arrow keys
        {"Up", IO::Key::Up},
        {"Down", IO::Key::Down},
        {"Left", IO::Key::Left},
        {"Right", IO::Key::Right},

        // Function keys
        {"F1", IO::Key::F1}, {"F2", IO::Key::F2}, {"F3", IO::Key::F3},
        {"F4", IO::Key::F4}, {"F5", IO::Key::F5}, {"F6", IO::Key::F6},
        {"F7", IO::Key::F7}, {"F8", IO::Key::F8}, {"F9", IO::Key::F9},
        {"F10", IO::Key::F10}, {"F11", IO::Key::F11}, {"F12", IO::Key::F12},
    };

    auto it = keyMap.find(keyName);
    if (it != keyMap.end()) {
        return it->second;
    }
    return -1;  // Unknown key
}

// Register all engine bindings to a Lua state
inline void registerBindings(sol::state& lua, ECS::Registry& registry) {

    // === Vec3 usertype (wrapper for glm::vec3) ===
    lua.new_usertype<glm::vec3>("Vec3",
        // Constructors
        sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),

        // Properties
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z,

        // Operators
        sol::meta_function::addition, [](const glm::vec3& a, const glm::vec3& b) { return a + b; },
        sol::meta_function::subtraction, [](const glm::vec3& a, const glm::vec3& b) { return a - b; },
        sol::meta_function::multiplication, sol::overload(
            [](const glm::vec3& a, float b) { return a * b; },
            [](float a, const glm::vec3& b) { return a * b; }
        ),
        sol::meta_function::division, [](const glm::vec3& a, float b) { return a / b; },
        sol::meta_function::unary_minus, [](const glm::vec3& a) { return -a; },
        sol::meta_function::to_string, [](const glm::vec3& v) {
            return "Vec3(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
        }
    );

    // === Vector math functions ===
    lua.set_function("vec3Length", [](const glm::vec3& v) { return glm::length(v); });
    lua.set_function("vec3Normalize", [](const glm::vec3& v) { return glm::normalize(v); });
    lua.set_function("vec3Dot", [](const glm::vec3& a, const glm::vec3& b) { return glm::dot(a, b); });
    lua.set_function("vec3Cross", [](const glm::vec3& a, const glm::vec3& b) { return glm::cross(a, b); });
    lua.set_function("vec3Distance", [](const glm::vec3& a, const glm::vec3& b) { return glm::distance(a, b); });
    lua.set_function("vec3Lerp", [](const glm::vec3& a, const glm::vec3& b, float t) { return glm::mix(a, b, t); });

    // === Transform API ===
    lua.set_function("getPosition", [&registry](uint64_t entityId) -> glm::vec3 {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            return transform->position;
        }
        return glm::vec3(0.0f);
    });

    lua.set_function("setPosition", [&registry](uint64_t entityId, float x, float y, float z) {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            transform->position = glm::vec3(x, y, z);
            transform->dirty = true;
        }
    });

    lua.set_function("setPositionVec", [&registry](uint64_t entityId, const glm::vec3& pos) {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            transform->position = pos;
            transform->dirty = true;
        }
    });

    lua.set_function("getRotation", [&registry](uint64_t entityId) -> glm::vec3 {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            return transform->rotation;
        }
        return glm::vec3(0.0f);
    });

    lua.set_function("setRotation", [&registry](uint64_t entityId, float x, float y, float z) {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            transform->rotation = glm::vec3(x, y, z);
            transform->dirty = true;
        }
    });

    lua.set_function("getScale", [&registry](uint64_t entityId) -> glm::vec3 {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            return transform->scale;
        }
        return glm::vec3(1.0f);
    });

    lua.set_function("setScale", [&registry](uint64_t entityId, float x, float y, float z) {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            transform->scale = glm::vec3(x, y, z);
            transform->dirty = true;
        }
    });

    // Convenience: translate position
    lua.set_function("translate", [&registry](uint64_t entityId, float dx, float dy, float dz) {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            transform->position += glm::vec3(dx, dy, dz);
            transform->dirty = true;
        }
    });

    // Convenience: rotate
    lua.set_function("rotate", [&registry](uint64_t entityId, float dx, float dy, float dz) {
        ECS::Entity entity{entityId};
        auto* transform = registry.get<ECS::TransformComponent>(entity);
        if (transform) {
            transform->rotation += glm::vec3(dx, dy, dz);
            transform->dirty = true;
        }
    });

    // === Input API ===
    lua.set_function("isKeyPressed", [](const std::string& keyName) -> bool {
        int key = stringToKey(keyName);
        if (key >= 0) {
            return IO::IOSystem::getInstance().isKeyPressed(key);
        }
        return false;
    });

    lua.set_function("isKeyJustPressed", [](const std::string& keyName) -> bool {
        int key = stringToKey(keyName);
        if (key >= 0) {
            return IO::IOSystem::getInstance().wasKeyJustPressed(key);
        }
        return false;
    });

    lua.set_function("isKeyJustReleased", [](const std::string& keyName) -> bool {
        int key = stringToKey(keyName);
        if (key >= 0) {
            return IO::IOSystem::getInstance().wasKeyJustReleased(key);
        }
        return false;
    });

    lua.set_function("getMousePosition", []() -> glm::vec3 {
        auto& mouse = IO::IOSystem::getInstance().getInputState().mouse;
        return glm::vec3(mouse.position.x, mouse.position.y, 0.0f);
    });

    lua.set_function("isMouseButtonPressed", [](int button) -> bool {
        return IO::IOSystem::getInstance().getInputState().mouse.isButtonPressed(button);
    });

    // === Time API ===
    lua.set_function("getDeltaTime", []() -> float {
        return GameState::getInstance().deltaTime;
    });

    lua.set_function("getTime", []() -> float {
        return GameState::getInstance().lastFrame;
    });

    // === Entity API ===
    lua.set_function("createEntity", [&registry]() -> uint64_t {
        ECS::Entity entity = registry.create();
        return entity.id;
    });

    lua.set_function("destroyEntity", [&registry](uint64_t entityId) {
        ECS::Entity entity{entityId};
        registry.destroy(entity);
    });

    lua.set_function("entityExists", [&registry](uint64_t entityId) -> bool {
        ECS::Entity entity{entityId};
        // Check if entity has any component (simple existence check)
        return registry.get<ECS::TransformComponent>(entity) != nullptr;
    });

    // === Component checks ===
    lua.set_function("hasTransform", [&registry](uint64_t entityId) -> bool {
        ECS::Entity entity{entityId};
        return registry.get<ECS::TransformComponent>(entity) != nullptr;
    });

    lua.set_function("hasMesh", [&registry](uint64_t entityId) -> bool {
        ECS::Entity entity{entityId};
        return registry.get<ECS::MeshComponent>(entity) != nullptr;
    });

    // === Visibility ===
    lua.set_function("setVisible", [&registry](uint64_t entityId, bool visible) {
        ECS::Entity entity{entityId};
        auto* render = registry.get<ECS::RenderComponent>(entity);
        if (render) {
            render->visible = visible;
        }
    });

    lua.set_function("isVisible", [&registry](uint64_t entityId) -> bool {
        ECS::Entity entity{entityId};
        auto* render = registry.get<ECS::RenderComponent>(entity);
        return render ? render->visible : false;
    });

    // === Debug/Utility ===
    lua.set_function("print", [](const std::string& msg) {
        std::cout << "[Lua] " << msg << std::endl;
    });

    lua.set_function("printVec3", [](const std::string& label, const glm::vec3& v) {
        std::cout << "[Lua] " << label << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    });

    // === Math utilities (already in Lua but convenient wrappers) ===
    lua.set_function("clamp", [](float value, float min, float max) -> float {
        return glm::clamp(value, min, max);
    });

    lua.set_function("lerp", [](float a, float b, float t) -> float {
        return glm::mix(a, b, t);
    });

    lua.set_function("randomFloat", [](float min, float max) -> float {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    });

    lua.set_function("randomInt", [](int min, int max) -> int {
        return min + rand() % (max - min + 1);
    });
}

} // namespace Scripting

#endif //GAMEENGINE_LUA_BINDINGS_H
