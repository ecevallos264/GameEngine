#include <thread>
#include "game.h"
#include "core/settings/settings.h"
#include "entities/Line.h"
#include "core/observability/FPSCounter.h"
#include "runtime/SystemManager.h"
#include "runtime/systems/InputSystem.h"
#include "runtime/systems/PhysicsSystem.h"
#include "runtime/systems/RenderSystem.h"
#include "runtime/systems/SceneSystem.h"

void Game::run(GLFWwindow* window) {
    SystemManager systemManager;
    systemManager.addSystem<InputSystem>();
    systemManager.addSystem<PhysicsSystem>();
    systemManager.addSystem<SceneSystem>();
    systemManager.addSystem<RenderSystem>();

    SystemContext context;
    context.window = window;
    systemManager.initializeAll(context);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        GameState::getInstance().deltaTime = currentFrame - GameState::getInstance().lastFrame;
        GameState::getInstance().lastFrame = currentFrame;

        context.deltaTime = GameState::getInstance().deltaTime;
        systemManager.updateAll(context);
        systemManager.renderAll(context);

//        FPSCounter::getInstance().increment();
//        std::cout << FPSCounter::getInstance().fps << std::endl;
//        glfwSetWindowTitle(window, std::to_string(FPSCounter::getInstance().getFPS()).c_str());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    systemManager.shutdownAll(context);
}
