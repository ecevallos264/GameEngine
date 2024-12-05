//
// Created by eceva on 11/21/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_SETTINGS_H
#define SOFTWAREENGINEERINGPROJECT_SETTINGS_H

#include "Singleton.h"

class Settings: public Singleton<Settings> {
public:
    float MAX_RENDER_DISTANCE = 1000.0f;
    bool CURSOR_FOCUS_STATUS = false;
    float CURSOR_SENSITIVITY = 0.1f;
};

#endif //SOFTWAREENGINEERINGPROJECT_SETTINGS_H
