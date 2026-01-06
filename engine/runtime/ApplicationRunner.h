#ifndef GAMEENGINE_APPLICATIONRUNNER_H
#define GAMEENGINE_APPLICATIONRUNNER_H

#include "IApplication.h"
#include "../platform/Window.h"

class ApplicationRunner {
public:
    static int run(IApplication* app);

private:
    static void mainLoop(IApplication* app, Window& window);
};

#endif //GAMEENGINE_APPLICATIONRUNNER_H
