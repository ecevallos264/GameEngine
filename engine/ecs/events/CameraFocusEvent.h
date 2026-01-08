#ifndef GAMEENGINE_CAMERAFOCUSEVENT_H
#define GAMEENGINE_CAMERAFOCUSEVENT_H

#include "../../eventing/Event.h"
#include "../../core/state/game_state.h"

namespace ECS {

struct CameraFocusEvent : public Event {
    bool focused;

    explicit CameraFocusEvent(bool focused)
        : Event(GameState::getInstance().deltaTime),
          focused(focused) {}
};

} // namespace ECS

#endif //GAMEENGINE_CAMERAFOCUSEVENT_H
