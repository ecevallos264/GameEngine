#ifndef GAME_ENGINE_SETTINGS_H
#define GAME_ENGINE_SETTINGS_H

#include "Singleton.h"

class Settings: public Singleton<Settings> {
public:
    float MAX_RENDER_DISTANCE = 200.0f;
    bool CURSOR_FOCUS_STATUS = false;
    float CURSOR_SENSITIVITY = 0.1f;
};

#endif //GAME_ENGINE_SETTINGS_H
