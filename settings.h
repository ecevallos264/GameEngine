//
// Created by eceva on 11/21/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_SETTINGS_H
#define SOFTWAREENGINEERINGPROJECT_SETTINGS_H

#include "Singleton.h"

class Settings: public Singleton<Settings> {
public:
    float MAX_RENDER_DISTANCE = 100.0f;
    bool CURSOR_FOCUS_STATUS = false;
};

#endif //SOFTWAREENGINEERINGPROJECT_SETTINGS_H
