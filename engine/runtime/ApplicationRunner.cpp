#include "ApplicationRunner.h"
#include <glad/glad.h>
#include <iostream>

int ApplicationRunner::run(IApplication* app) {
    if (!app) {
        std::cerr << "ApplicationRunner: No application provided" << std::endl;
        return -1;
    }

    ApplicationConfig config;
    app->configure(config);
    config.applyToWindowConfig();

    Window window;
    if (!window.initialize(config.window)) {
        std::cerr << "ApplicationRunner: Failed to initialize window" << std::endl;
        return -1;
    }

    // Start with cursor visible for editor mode
    window.setCursorMode(CursorMode::Normal);

    app->initialize(window);

    mainLoop(app, window);

    app->shutdown();
    window.shutdown();

    return 0;
}

void ApplicationRunner::mainLoop(IApplication* app, Window& window) {
    float lastFrame = 0.0f;

    while (!window.shouldClose()) {
        float currentFrame = window.getTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        app->update(deltaTime);
        app->render();

        window.swapBuffers();
        window.pollEvents();
    }
}
