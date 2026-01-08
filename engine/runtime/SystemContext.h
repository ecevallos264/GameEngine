#ifndef GAMEENGINE_SYSTEM_CONTEXT_H
#define GAMEENGINE_SYSTEM_CONTEXT_H

class Window;

namespace ECS {
    class CameraSystem;
}

struct SystemContext {
    Window* window = nullptr;
    float deltaTime = 0.0f;

    // ECS camera system reference (for render systems)
    ECS::CameraSystem* cameraSystem = nullptr;
};

#endif //GAMEENGINE_SYSTEM_CONTEXT_H
