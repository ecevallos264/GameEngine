#ifndef GAMEENGINE_RENDER_SYSTEM_H
#define GAMEENGINE_RENDER_SYSTEM_H

#include "../ISystem.h"
#include "../../camera/CameraHandler.h"
#include "../../rendering/SceneController.h"

class RenderSystem : public ISystem {
public:
    void render(SystemContext& context) override {
        (void)context;
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene) {
            return;
        }
        auto camera = CameraHandler::getInstance().getCamera();
        if (!camera) {
            return;
        }
        currentScene->render(camera->getViewMatrix(), camera->getProjectionMatrix());
    }
};

#endif //GAMEENGINE_RENDER_SYSTEM_H
