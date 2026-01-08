#ifndef GAMEENGINE_RENDER_SYSTEM_H
#define GAMEENGINE_RENDER_SYSTEM_H

#include "../ISystem.h"
#include "../../rendering/SceneController.h"
#include "../../ecs/systems/CameraSystem.h"

class RenderSystem : public ISystem {
public:
    void render(SystemContext& context) override {
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene) {
            return;
        }

        // Use CameraSystem from context
        if (!context.cameraSystem) {
            return;
        }

        if (!context.cameraSystem->getActiveCamera().isValid()) {
            return;
        }

        glm::mat4 view = context.cameraSystem->getViewMatrix();
        glm::mat4 projection = context.cameraSystem->getProjectionMatrix();

        currentScene->render(view, projection);
    }
};

#endif //GAMEENGINE_RENDER_SYSTEM_H
