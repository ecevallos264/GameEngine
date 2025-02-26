//
// Created by eceva on 1/5/2025.
//
#include "../patterns/Singleton.h"

#ifndef GAMEENGINE_SETTINGS_H
#define GAMEENGINE_SETTINGS_H

class Settings : Singleton<Settings> {
public:
    constexpr static double IN_GAME_SENSITIVITY = 2.5f;
    constexpr static double IN_GAME_RUNNING_SENSITIVITY_FACTOR = 2.0f;

    constexpr static float MAX_RENDER_DISTANCE = 20.0f;
//    constexpr static bool CURSOR_FOCUS_STATUS = false;

    constexpr static float CURSOR_SENSITIVITY = 0.1f;

    constexpr static int WINDOW_WIDTH = 1600;
    constexpr static int WINDOW_HEIGHT = 1200;
    constexpr static char* WINDOW_TITLE = "Open GL Learning Material";

    constexpr static int MAX_GJK_ITERATION = 1000;

    constexpr static bool DEBUG_RENDERING = false;

    constexpr static glm::vec3 BOUNDING_REGION_COLOR = glm::vec3(1.0f);
};

#endif //GAMEENGINE_SETTINGS_H
