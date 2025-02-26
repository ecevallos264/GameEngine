//
// Created by eceva on 2/23/2025.
//

#ifndef FLAGSTATE_H
#define FLAGSTATE_H
#include <unordered_map>

enum EntityFlags {
    ENTITY_ROTATED,
    ENTITY_SCALED,
    ENTITY_MOVED,
    ENTITY_COLLIDED,
    ENTITY_DESTROYED,
    ENTITY_CREATED,
    ENTITY_FIRST_UPDATE,
};

class FlagState {
private:
    std::unordered_map<EntityFlags, bool> flags;
public:
    FlagState() = default;
    virtual ~FlagState() = default;

    bool get(EntityFlags flag) {
        if (flags.find(flag) == flags.end()) {
            flags[flag] = false;
        }
        return flags[flag];
    }

    bool getAndReset(EntityFlags flag) {
        bool value = get(flag);
        flags[flag] = false;
        return value;
    }

    void set(EntityFlags flag) {
        flags[flag] = true;
    }
};

#endif //FLAGSTATE_H
