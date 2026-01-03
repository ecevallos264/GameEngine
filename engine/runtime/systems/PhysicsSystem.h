#ifndef GAMEENGINE_PHYSICS_SYSTEM_H
#define GAMEENGINE_PHYSICS_SYSTEM_H

#include "../ISystem.h"

class PhysicsSystem : public ISystem {
public:
    void update(SystemContext& context) override {
        (void)context;
    }
};

#endif //GAMEENGINE_PHYSICS_SYSTEM_H
