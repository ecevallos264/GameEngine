#include <thread>
#include "game.h"
#include "camera/CameraHandler.h"
#include "core/settings/settings.h"
#include "entitities/Line.h"
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

//        for(int i = 0; i < SceneController::getInstance().getCurrentScene()->getEntityController()->entities.size(); i++) {
//            Entity* entity1 = SceneController::getInstance().getCurrentScene()->getEntityController()->entities[i];
//            if(entity1->fixed) continue;
//            for(int j = i + 1; j < SceneController::getInstance().getCurrentScene()->getEntityController()->entities.size(); j++) {
//                Entity* entity2 = SceneController::getInstance().getCurrentScene()->getEntityController()->entities[j];
//                if(entity2->fixed) continue;
//                Shape* shape1 = static_cast<Shape*>(entity1);
//                Shape* shape2 = static_cast<Shape*>(entity2);
//                if(CollisionHandler::getInstance().checkForCollision(shape1, shape2)) {
//                    MyCube* cube1 = static_cast<MyCube*>(entity1);
//                    MyCube* cube2 = static_cast<MyCube*>(entity2);
////                    std::cout << "Found" << (cube1)->id << ":" << (cube2)->id << std::endl;
//                    cube1->colliding = true;
//                    cube2->colliding = true;
//                }
//            }
//        }

        SceneController::getInstance().getCurrentScene()->render(CameraHandler::getInstance().getCamera()->getViewMatrix(), CameraHandler::getInstance().getCamera()->getProjectionMatrix());

//        FPSCounter::getInstance().increment();
//        std::cout << FPSCounter::getInstance().fps << std::endl;
//        glfwSetWindowTitle(window, std::to_string(FPSCounter::getInstance().getFPS()).c_str());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}