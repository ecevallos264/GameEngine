#ifndef GAMEENGINE_INPUTHANDLER_H
#define GAMEENGINE_INPUTHANDLER_H

#include <unordered_map>
#include <memory>
#include <functional>
#include "../core/patterns/Singleton.h"
#include "../io/IOSystem.h"

// Legacy compatibility - delegates to IOSystem
class InputHandler : public Singleton<InputHandler> {
public:
    InputHandler() : Singleton<InputHandler>() {}

    // Check if a key is currently pressed
    static bool isKeyActive(int key) {
        return IO::IOSystem::getInstance().isKeyPressed(key);
    }

    // Check if key was just pressed this frame
    static bool wasKeyJustPressed(int key) {
        return IO::IOSystem::getInstance().wasKeyJustPressed(key);
    }

    // Check if key was just released this frame
    static bool wasKeyJustReleased(int key) {
        return IO::IOSystem::getInstance().wasKeyJustReleased(key);
    }

    // Mouse queries
    static bool isMouseButtonPressed(int button) {
        return IO::IOSystem::getInstance().isMouseButtonPressed(button);
    }

    static glm::vec2 getMousePosition() {
        return IO::IOSystem::getInstance().getMousePosition();
    }

    static glm::vec2 getMouseDelta() {
        return IO::IOSystem::getInstance().getMouseDelta();
    }

    static glm::vec2 getScrollDelta() {
        return IO::IOSystem::getInstance().getScrollDelta();
    }
};

#endif //GAMEENGINE_INPUTHANDLER_H
