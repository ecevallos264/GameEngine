#ifndef GAMEENGINE_APPLICATIONCONFIG_H
#define GAMEENGINE_APPLICATIONCONFIG_H

#include <string>
#include "../platform/WindowConfig.h"

struct ApplicationConfig {
    WindowConfig window;

    int windowWidth = 1280;
    int windowHeight = 720;
    std::string windowTitle = "Game Engine";
    bool vsync = false;

    void applyToWindowConfig() {
        window.width = windowWidth;
        window.height = windowHeight;
        window.title = windowTitle;
        window.vsync = vsync;
    }
};

#endif //GAMEENGINE_APPLICATIONCONFIG_H
