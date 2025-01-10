//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERAHANDLER_H
#define GAMEENGINE_CAMERAHANDLER_H

#include "../utils/patterns/Singleton.h"
#include "Camera.h"
#include "../eventing/EventListener.h"
#include "../eventing/EventDispatcher.h"

class CameraHandler : public Singleton<CameraHandler>, public EventListener {
private:
    Camera camera;
public:
    CameraHandler() : Singleton<CameraHandler>() {
        EventDispatcher::getInstance().registerListener<>()
    }

    void setCamera(Camera camera);

    Camera getCamera();

    void updateCamera(double posX, double posY);
};

#endif //GAMEENGINE_CAMERAHANDLER_H
