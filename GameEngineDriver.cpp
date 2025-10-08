#include "GameEngine.h"
#include <iostream>

void testGameStates() {
    GameEngine engine;
    std::cout << "=== testGameStates() ===\n";
    std::cout << engine << "\n";
    engine.runConsole();
}

//int main() {
//    testGameStates();
//    return 0;
//}
