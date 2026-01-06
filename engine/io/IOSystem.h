#ifndef GAMEENGINE_IO_SYSTEM_H
#define GAMEENGINE_IO_SYSTEM_H

#include "../runtime/ISystem.h"
#include "../platform/Window.h"
#include "../core/patterns/Singleton.h"
#include "InputState.h"

namespace IO {

class IOSystem : public ISystem, public Singleton<IOSystem> {
public:
    IOSystem() : Singleton<IOSystem>() {}

    void initialize(SystemContext& context) override {
        if (!context.window) return;

        // Set up keyboard callback
        context.window->setKeyCallback([this](int key, int scancode, int action, int mods) {
            (void)scancode;
            (void)mods;
            inputState.keyboard.setKeyState(key, action);
        });

        // Set up mouse move callback
        context.window->setMouseMoveCallback([this](double x, double y) {
            inputState.mouse.setPosition(x, y);
        });

        // Set up mouse button callback
        context.window->setMouseButtonCallback([this](int button, int action, int mods) {
            (void)mods;
            inputState.mouse.setButtonState(button, action);
        });

        // Set up scroll callback
        context.window->setScrollCallback([this](double xOffset, double yOffset) {
            inputState.mouse.setScrollDelta(xOffset, yOffset);
        });
    }

    void update(SystemContext& context) override {
        (void)context;
        // Clear per-frame state at the end of update
        // This allows systems that run after to still see "just pressed" states
    }

    void shutdown(SystemContext& context) override {
        (void)context;
        // Clear callbacks
    }

    // Call this at the end of the frame to reset per-frame input states
    void endFrame() {
        inputState.clearFrameState();
    }

    // Keyboard queries
    bool isKeyPressed(int key) const {
        return inputState.keyboard.isPressed(key);
    }

    bool wasKeyJustPressed(int key) const {
        return inputState.keyboard.wasJustPressed(key);
    }

    bool wasKeyJustReleased(int key) const {
        return inputState.keyboard.wasJustReleased(key);
    }

    // Mouse queries
    bool isMouseButtonPressed(int button) const {
        return inputState.mouse.isButtonPressed(button);
    }

    bool wasMouseButtonJustPressed(int button) const {
        return inputState.mouse.wasButtonJustPressed(button);
    }

    bool wasMouseButtonJustReleased(int button) const {
        return inputState.mouse.wasButtonJustReleased(button);
    }

    glm::vec2 getMousePosition() const {
        return inputState.mouse.position;
    }

    glm::vec2 getMouseDelta() const {
        return inputState.mouse.delta;
    }

    glm::vec2 getScrollDelta() const {
        return inputState.mouse.scrollDelta;
    }

    // Direct access to state for advanced use cases
    const InputState& getInputState() const {
        return inputState;
    }

    InputState& getInputState() {
        return inputState;
    }

private:
    InputState inputState;
};

} // namespace IO

#endif //GAMEENGINE_IO_SYSTEM_H
