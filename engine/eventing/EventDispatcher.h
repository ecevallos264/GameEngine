//
// Created by eceva on 1/6/2025.
//

#ifndef GAMEENGINE_EVENTDISPATCHER_H
#define GAMEENGINE_EVENTDISPATCHER_H

#include <typeindex>
#include <unordered_map>
#include <vector>
#include <functional>
#include "Event.h"
#include "EventListener.h"

/**
 * TODO Have 2 types of dispatching:
 * Immediate - event is dispatched as soon as it's fired
 * Queued - event is queued and dispatched at the end of the frame
 * */

class EventDispatcher : public Singleton<EventDispatcher> {
private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> listeners;
public:
    EventDispatcher() : Singleton<EventDispatcher>() {};

    template<typename EventType>
    void registerListener(std::function<void(const Event&)> callback) {
        listeners[std::type_index(typeid(EventType))].push_back(callback);
    }

    template<typename EventType>
    void registerListener(EventListener* listener) {
        listeners[std::type_index(typeid(EventType))].push_back([listener](Event event) {
            listener->onEvent(event);
        });
    }

    void dispatch(const Event& event) {
        auto type = std::type_index(typeid(event));
        if (listeners.find(type) != listeners.end()) {
            for(auto& callback : listeners[type])
                callback(event);
        }
    }
};

#endif //GAMEENGINE_EVENTDISPATCHER_H
