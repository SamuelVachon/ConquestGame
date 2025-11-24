#include "GameEngine.h"
#include <vector>
#include <string>
#include <iostream>

void testGameStates() {
    GameEngine engine;
    std::cout << "=== testGameStates() ===\n";
    std::cout << engine << "\n";
    engine.runConsole();
}

void testTournament() {
    GameEngine engine;

    // TODO: put your real .map filenames here
    std::vector<std::string> maps = {
        "canada.map",
        "europe.map"
    };

    // Strategies for players (one per player)
    std::vector<std::string> strategies = {
        "aggressive",
        "benevolent",
        "neutral"
        // you can add "cheater", "human", etc.
    };

    int G = 3;   // games per map
    int D = 10;  // max turns per game

    engine.runTournament(maps, strategies, G, D);
}
