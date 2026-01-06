#ifndef GAMEENGINE_WINDOWCONFIG_H
#define GAMEENGINE_WINDOWCONFIG_H

#include <string>

enum class CursorMode {
    Normal,
    Hidden,
    Disabled
};

namespace KeyAction {
    constexpr int Release = 0;
    constexpr int Press = 1;
    constexpr int Repeat = 2;
}

struct WindowConfig {
    int width = 1280;
    int height = 720;
    std::string title = "Game Engine";
    bool vsync = false;
    bool resizable = true;
    int glMajorVersion = 3;
    int glMinorVersion = 3;
};

#endif //GAMEENGINE_WINDOWCONFIG_H
