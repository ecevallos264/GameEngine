#ifndef GAMEENGINE_INPUTHANDLER_H
#define GAMEENGINE_INPUTHANDLER_H

#include <glfw/glfw3.h>
#include <unordered_map>
#include <memory>
#include <functional>
#include "../patterns/Singleton.h"

struct KeyState {
    bool state = false;
    bool isDirty = false;
};

struct KeyboardState {
    std::unordered_map<int, std::unique_ptr<KeyState>> keyState;
};

class InputHandler : public Singleton<InputHandler> {
protected:
    KeyboardState keyboardState;

    // window, xPos, yPos
    std::function<void(GLFWwindow*, int, int)> onMouseMovementCallback;
    // window, int key, int scancode, int action, int mods
    std::function<void(GLFWwindow*, int, int, int, int)> onKeyPressCallback;
public:
    InputHandler() : Singleton<InputHandler>() {};
    static void setKeyState(int key, bool state);
    static void setKeyDirty(int key);

    static bool isKeyActive(int key);
    static bool isKeyDirty(int key);

    // Parameters are window, xPos, yPos
    static void setMouseMovementCallback(GLFWwindow* window, std::function<void(GLFWwindow*, int, int)>);
    static void setKeyPressCallback(GLFWwindow* window, std::function<void(GLFWwindow*, int, int, int, int)>);
};

#endif //GAMEENGINE_INPUTHANDLER_H
