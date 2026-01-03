#ifndef GAMEENGINE_ISYSTEM_H
#define GAMEENGINE_ISYSTEM_H

#include "SystemContext.h"

class ISystem {
public:
    virtual ~ISystem() = default;

    virtual void initialize(SystemContext& context) {}
    virtual void shutdown(SystemContext& context) {}

    virtual void update(SystemContext& context) {}
    virtual void fixedUpdate(SystemContext& context) {}
    virtual void render(SystemContext& context) {}
};

#endif //GAMEENGINE_ISYSTEM_H
