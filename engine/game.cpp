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

        SceneController::getInstance().getCurrentScene()->handleInput(window);
        SceneController::getInstance().getCurrentScene()->update(GameState::getInstance().deltaTime);

        SceneController::getInstance().getCurrentScene()->render(CameraHandler::getInstance().getCamera()->getViewMatrix(), CameraHandler::getInstance().getCamera()->getProjectionMatrix());

//        FPSCounter::getInstance().increment();
//        std::cout << FPSCounter::getInstance().fps << std::endl;
//        glfwSetWindowTitle(window, std::to_string(FPSCounter::getInstance().getFPS()).c_str());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}