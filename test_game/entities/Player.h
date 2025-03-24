// Player.h
//
// Created by eceva on 1/7/2025.
//

#ifndef GAMEENGINE_PLAYER_H
#define GAMEENGINE_PLAYER_H

#include <iostream>
#include "../../engine/core/eventing/EventListener.h"
#include "../../engine/core/eventing/events/KeyEvent.h"
#include "../../engine/core/state/game_state.h"
#include "../../engine/input/InputHandler.h"
#include "../../engine/core/eventing/events/CursorEvent.h"
#include "../../engine/core/eventing/events/CameraKeyMovementEvent.h"
#include "../../engine/input/MouseHandler.h"
#include "../../engine/rendering/Entity.h"
#include "../../engine/physics/RigidBody.h"
#include "../../engine/core/shaders/ShaderManager.h"

class Player : public EventListener, public RigidBody {
public:
    Player() : EventListener(), RigidBody(ShaderManager::getInstance().getShader("shader1")) {
        EventDispatcher::getInstance().registerListener<KeyEvent>([this](const Event& event) {
            this->onEvent(static_cast<const KeyEvent&>(event));
        });
    }

    void onEvent(const Event &event) {
    }

    void onEvent(const KeyEvent &event) {

    }

    void onEvent(const CursorEvent &event) {
        //...
    }

    int update(float deltaTime) override {
        return 0;
    }

    std::type_index getType() {
        return typeid(Player);
    }
};

#endif //GAMEENGINE_PLAYER_H