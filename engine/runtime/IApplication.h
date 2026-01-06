#ifndef GAMEENGINE_IAPPLICATION_H
#define GAMEENGINE_IAPPLICATION_H

#include "ApplicationConfig.h"

class Window;

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void configure(ApplicationConfig& config) = 0;

    virtual void initialize(Window& window) = 0;

    virtual void update(float deltaTime) = 0;

    virtual void render() = 0;

    virtual void shutdown() = 0;
};

#endif //GAMEENGINE_IAPPLICATION_H
