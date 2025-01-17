#include "game.h"
#include "camera/CameraHandler.h"
#include "core/settings/settings.h"
#include "entitities/line.h"
#include "core/observability/FPSCounter.h"
#include "rendering/SceneController.h"
#include "physics/CollisionHandler.h"

void Game::run(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        GameState::getInstance().deltaTime = currentFrame - GameState::getInstance().lastFrame;
        GameState::getInstance().lastFrame = currentFrame;

        SceneController::getInstance().getCurrentScene()->update(GameState::getInstance().deltaTime);

        for(int i = 0; i < SceneController::getInstance().getCurrentScene()->getEntityController()->entities.size(); i++) {
            Entity* entity1 = SceneController::getInstance().getCurrentScene()->getEntityController()->entities[i];
            if(entity1->fixed) continue;
            for(int j = i + 1; j < SceneController::getInstance().getCurrentScene()->getEntityController()->entities.size(); j++) {
                Entity* entity2 = SceneController::getInstance().getCurrentScene()->getEntityController()->entities[j];
                if(entity2->fixed) continue;
                std::cout << "Checking Collision[" << i << ":" << j << "] with " << entity1->getInstanceId() << " and " << entity2->getInstanceId() << std::endl;
                if(CollisionHandler::getInstance().handleCollision(entity1, entity2)) {
                    EventDispatcher::getInstance().dispatch(
                            CollisionEvent(
                                    GameState::getInstance().deltaTime,
                                    entity1,
                                    entity2));
                }
            }
        }

        SceneController::getInstance().getCurrentScene()->render(CameraHandler::getInstance().getCamera()->getViewMatrix(), CameraHandler::getInstance().getCamera()->getProjectionMatrix());

        FPSCounter::getInstance().increment();
//        std::cout << FPSCounter::getInstance().fps << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}