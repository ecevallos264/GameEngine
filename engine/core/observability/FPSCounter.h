#ifndef GAMEENGINE_FPSCOUNTER_H
#define GAMEENGINE_FPSCOUNTER_H

#include "../patterns/Singleton.h"
#include <iostream>
#include <chrono>

class FPSCounter : public Singleton<FPSCounter> {
public:
    unsigned int fps;
    unsigned int prevFPS;
private:
    unsigned int frameCount;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

public:
    FPSCounter() : Singleton<FPSCounter>(), frameCount(0), fps(0), lastTime(std::chrono::high_resolution_clock::now()) {}

    void increment() {
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        if (elapsed.count() >= 1.0) {
            prevFPS = fps;
            fps = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    unsigned int getFPS() {
        return fps;
    }
};

#endif //GAMEENGINE_FPSCOUNTER_H