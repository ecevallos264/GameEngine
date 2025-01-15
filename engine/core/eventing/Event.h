//
// Created by eceva on 1/6/2025.
//

#ifndef GAMEENGINE_EVENT_H
#define GAMEENGINE_EVENT_H

struct Event {
    float deltaTime;
    explicit Event(float deltaTime) : deltaTime(deltaTime) {};
    virtual ~Event() = default;
};

#endif //GAMEENGINE_EVENT_H
