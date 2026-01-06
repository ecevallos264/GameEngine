#include "../engine/runtime/ApplicationRunner.h"
#include "TestApplication.h"

int main() {
    TestApplication app;
    return ApplicationRunner::run(&app);
}
