#ifndef GAMEENGINE_INPUT_STATE_H
#define GAMEENGINE_INPUT_STATE_H

#include <unordered_map>
#include <unordered_set>
#include <glm/vec2.hpp>

namespace IO {

// Key codes that mirror GLFW but don't require GLFW header
namespace Key {
    constexpr int Space = 32;
    constexpr int Apostrophe = 39;
    constexpr int Comma = 44;
    constexpr int Minus = 45;
    constexpr int Period = 46;
    constexpr int Slash = 47;
    constexpr int Num0 = 48;
    constexpr int Num1 = 49;
    constexpr int Num2 = 50;
    constexpr int Num3 = 51;
    constexpr int Num4 = 52;
    constexpr int Num5 = 53;
    constexpr int Num6 = 54;
    constexpr int Num7 = 55;
    constexpr int Num8 = 56;
    constexpr int Num9 = 57;
    constexpr int Semicolon = 59;
    constexpr int Equal = 61;
    constexpr int A = 65;
    constexpr int B = 66;
    constexpr int C = 67;
    constexpr int D = 68;
    constexpr int E = 69;
    constexpr int F = 70;
    constexpr int G = 71;
    constexpr int H = 72;
    constexpr int I = 73;
    constexpr int J = 74;
    constexpr int K = 75;
    constexpr int L = 76;
    constexpr int M = 77;
    constexpr int N = 78;
    constexpr int O = 79;
    constexpr int P = 80;
    constexpr int Q = 81;
    constexpr int R = 82;
    constexpr int S = 83;
    constexpr int T = 84;
    constexpr int U = 85;
    constexpr int V = 86;
    constexpr int W = 87;
    constexpr int X = 88;
    constexpr int Y = 89;
    constexpr int Z = 90;
    constexpr int Escape = 256;
    constexpr int Enter = 257;
    constexpr int Tab = 258;
    constexpr int Backspace = 259;
    constexpr int Insert = 260;
    constexpr int Delete = 261;
    constexpr int Right = 262;
    constexpr int Left = 263;
    constexpr int Down = 264;
    constexpr int Up = 265;
    constexpr int PageUp = 266;
    constexpr int PageDown = 267;
    constexpr int Home = 268;
    constexpr int End = 269;
    constexpr int CapsLock = 280;
    constexpr int ScrollLock = 281;
    constexpr int NumLock = 282;
    constexpr int PrintScreen = 283;
    constexpr int Pause = 284;
    constexpr int F1 = 290;
    constexpr int F2 = 291;
    constexpr int F3 = 292;
    constexpr int F4 = 293;
    constexpr int F5 = 294;
    constexpr int F6 = 295;
    constexpr int F7 = 296;
    constexpr int F8 = 297;
    constexpr int F9 = 298;
    constexpr int F10 = 299;
    constexpr int F11 = 300;
    constexpr int F12 = 301;
    constexpr int LeftShift = 340;
    constexpr int LeftControl = 341;
    constexpr int LeftAlt = 342;
    constexpr int LeftSuper = 343;
    constexpr int RightShift = 344;
    constexpr int RightControl = 345;
    constexpr int RightAlt = 346;
    constexpr int RightSuper = 347;
}

namespace MouseButton {
    constexpr int Left = 0;
    constexpr int Right = 1;
    constexpr int Middle = 2;
}

namespace Action {
    constexpr int Release = 0;
    constexpr int Press = 1;
    constexpr int Repeat = 2;
}

struct KeyboardState {
    std::unordered_set<int> pressedKeys;
    std::unordered_set<int> justPressedKeys;   // Keys pressed this frame
    std::unordered_set<int> justReleasedKeys;  // Keys released this frame

    bool isPressed(int key) const {
        return pressedKeys.find(key) != pressedKeys.end();
    }

    bool wasJustPressed(int key) const {
        return justPressedKeys.find(key) != justPressedKeys.end();
    }

    bool wasJustReleased(int key) const {
        return justReleasedKeys.find(key) != justReleasedKeys.end();
    }

    void setKeyState(int key, int action) {
        if (action == Action::Press) {
            if (pressedKeys.find(key) == pressedKeys.end()) {
                justPressedKeys.insert(key);
            }
            pressedKeys.insert(key);
        } else if (action == Action::Release) {
            pressedKeys.erase(key);
            justReleasedKeys.insert(key);
        }
    }

    void clearFrameState() {
        justPressedKeys.clear();
        justReleasedKeys.clear();
    }
};

struct MouseState {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 lastPosition = glm::vec2(0.0f);
    glm::vec2 delta = glm::vec2(0.0f);
    glm::vec2 scrollDelta = glm::vec2(0.0f);

    std::unordered_set<int> pressedButtons;
    std::unordered_set<int> justPressedButtons;
    std::unordered_set<int> justReleasedButtons;

    bool firstMove = true;

    bool isButtonPressed(int button) const {
        return pressedButtons.find(button) != pressedButtons.end();
    }

    bool wasButtonJustPressed(int button) const {
        return justPressedButtons.find(button) != justPressedButtons.end();
    }

    bool wasButtonJustReleased(int button) const {
        return justReleasedButtons.find(button) != justReleasedButtons.end();
    }

    void setButtonState(int button, int action) {
        if (action == Action::Press) {
            if (pressedButtons.find(button) == pressedButtons.end()) {
                justPressedButtons.insert(button);
            }
            pressedButtons.insert(button);
        } else if (action == Action::Release) {
            pressedButtons.erase(button);
            justReleasedButtons.insert(button);
        }
    }

    void setPosition(double x, double y) {
        lastPosition = position;
        position = glm::vec2(static_cast<float>(x), static_cast<float>(y));

        if (firstMove) {
            lastPosition = position;
            firstMove = false;
        }

        delta = position - lastPosition;
    }

    void setScrollDelta(double xOffset, double yOffset) {
        scrollDelta = glm::vec2(static_cast<float>(xOffset), static_cast<float>(yOffset));
    }

    void clearFrameState() {
        justPressedButtons.clear();
        justReleasedButtons.clear();
        delta = glm::vec2(0.0f);
        scrollDelta = glm::vec2(0.0f);
    }
};

struct InputState {
    KeyboardState keyboard;
    MouseState mouse;

    void clearFrameState() {
        keyboard.clearFrameState();
        mouse.clearFrameState();
    }
};

} // namespace IO

#endif //GAMEENGINE_INPUT_STATE_H
