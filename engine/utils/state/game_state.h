//
// Created by eceva on 1/5/2025.
//

#ifndef GAMEENGINE_GAME_STATE_H
#define GAMEENGINE_GAME_STATE_H

class GameState : public Singleton<GameState>{
public:
    GameState() : Singleton<GameState>() {};
    //Mouse State
    bool CURSOR_FOCUS_STATUS = false;
    double lastX, lastY;
    bool firstMouse = true;
    int xPos, yPos;

    //Rendering State
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif //GAMEENGINE_GAME_STATE_H
