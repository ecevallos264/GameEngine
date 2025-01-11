//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERAHANDLER_H
#define GAMEENGINE_CAMERAHANDLER_H

#include "../utils/patterns/Singleton.h"
#include "Camera.h"
#include "../eventing/EventListener.h"
#include "../eventing/EventDispatcher.h"
#include "../eventing/events/CameraMovementEvent.h"

class CameraHandler : public Singleton<CameraHandler>, public EventListener {
private:
    Camera* camera;
public:
    CameraHandler() : Singleton<CameraHandler>() {
        EventDispatcher::getInstance().registerListener<CameraMovementEvent>([this](const Event& event) {
            this->onEvent(dynamic_cast<const CameraMovementEvent&>(event));
        });
    }

    void setCamera(Camera* camera);

    Camera* getCamera();

    void onEvent(const Event& event) override;
    void onEvent(const CameraMovementEvent& event);
};

#endif //GAMEENGINE_CAMERAHANDLER_H
