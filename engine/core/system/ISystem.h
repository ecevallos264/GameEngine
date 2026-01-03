//
// Created by eceva on 1/2/2026.
//

#ifndef GAMEENGINE_ISYSTEM_H
#define GAMEENGINE_ISYSTEM_H

/**
 * Physics System
 * Rendering System
 * Input System
 * */

struct ISystem {
    virtual ~ISystem() = default;

    void initialize() {};
    void update(float deltaTime) {};
    void shutdown() {};
};

#endif //GAMEENGINE_ISYSTEM_H
