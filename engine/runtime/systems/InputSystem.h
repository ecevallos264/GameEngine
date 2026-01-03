#ifndef GAMEENGINE_INPUT_SYSTEM_H
#define GAMEENGINE_INPUT_SYSTEM_H

#include "../ISystem.h"
#include "../../rendering/SceneController.h"

class InputSystem : public ISystem {
public:
    void update(SystemContext& context) override {
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene || !context.window) {
            return;
        }
        currentScene->handleInput(context.window);
    }
};

#endif //GAMEENGINE_INPUT_SYSTEM_H
