#ifndef GAMEENGINE_INPUTHANDLER_H
#define GAMEENGINE_INPUTHANDLER_H

#include <unordered_map>
#include <memory>
#include "../patterns/Singleton.h"

struct KeyState {
    bool state = false;
    bool isDirty = false;
};


/**
 * Dirty is a term used to describe if a key has been pressed and released recently but has not been handled
 * */
class InputHandler : public Singleton<InputHandler> {
protected:
    std::unordered_map<int, std::unique_ptr<KeyState>> keyState;
public:
    InputHandler() : Singleton<InputHandler>() {};
    static void setKeyState(int key, bool state);
    static void setKeyDirty(int key);

    static bool isKeyActive(int key);
    static bool isKeyDirty(int key);
};

#endif //GAMEENGINE_INPUTHANDLER_H
