#ifndef GAMEENGINE_SCENE_SYSTEM_H
#define GAMEENGINE_SCENE_SYSTEM_H

#include "../ISystem.h"
#include "../../rendering/SceneController.h"

class SceneSystem : public ISystem {
public:
    void update(SystemContext& context) override {
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene) {
            return;
        }
        currentScene->update(context.deltaTime);
    }
};

#endif //GAMEENGINE_SCENE_SYSTEM_H
