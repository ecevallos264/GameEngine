#ifndef GAMEENGINE_INPUT_SYSTEM_H
#define GAMEENGINE_INPUT_SYSTEM_H

#include "../ISystem.h"
#include "../../rendering/SceneController.h"
#include "../../platform/Window.h"
#include <glfw/glfw3.h>

class InputSystem : public ISystem {
public:
    void update(SystemContext& context) override {
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene || !context.window) {
            return;
        }
        auto* glfwWindow = static_cast<GLFWwindow*>(context.window->getNativeHandle());
        currentScene->handleInput(glfwWindow);
    }
};

#endif //GAMEENGINE_INPUT_SYSTEM_H
