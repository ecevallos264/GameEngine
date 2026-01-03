//
// Created by eceva on 1/2/2026.
//

#ifndef GAMEENGINE_ISYSTEM_H
#define GAMEENGINE_ISYSTEM_H

/**
 *
 * */

struct ISystem {
    virtual ~ISystem() = default;

    void initialize() {};
    void update(float deltaTime) {};
    void shutdown() {};
};

#endif //GAMEENGINE_ISYSTEM_H
