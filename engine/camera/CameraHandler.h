//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERAHANDLER_H
#define GAMEENGINE_CAMERAHANDLER_H

#include "../core/patterns/Singleton.h"
#include "Camera.h"
#include "../core/eventing/EventListener.h"
#include "../core/eventing/EventDispatcher.h"
#include "../core/eventing/events/CameraKeyMovementEvent.h"
#include "../core/eventing/events/MouseMovementEvent.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

class CameraHandler : public Singleton<CameraHandler>, public EventListener {
private:
    Camera* camera;
public:
    CameraHandler() : Singleton<CameraHandler>() {
        EventDispatcher::getInstance().registerListener<CameraKeyMovementEvent>([this](const Event& event) {
            this->onEvent(dynamic_cast<const CameraKeyMovementEvent&>(event));
        });
        EventDispatcher::getInstance().registerListener<MouseMovementEvent>([this](const Event& event) {
            this->onEvent(dynamic_cast<const MouseMovementEvent&>(event));
        });
    }

    void setCamera(Camera* camera);

    Camera* getCamera();



    void onEvent(const Event& event) override;
    void onEvent(const CameraKeyMovementEvent& event);
    void onEvent(const MouseMovementEvent& event);
};

#endif //GAMEENGINE_CAMERAHANDLER_H
