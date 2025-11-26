#include "GameEngine.h"
#include "CommandProcessing.h"
#include <vector>
#include <string>
#include <iostream>

void testGameStates() {
    GameEngine engine;
    std::cout << "testGameStates()\n";
    std::cout << engine << "\n";
    engine.runConsole();
}

void testTournament() {
    std::cout << "testTournament() \n";

    // 1) Build a tournament command line (this should match the maps/strategies you pass to GameEngine)
    std::string cmdLine = "tournament -M canada.map,europe.map "
        "-P aggressive,benevolent,neutral "
        "-G 3 -D 10";

    // 2) Let the CommandProcessor process & validate the command
    CommandProcessor cp;
    Command* c = new Command(cmdLine);
    cp.validate(c);

    std::cout << "Processed command: " << c->getRaw() << "\n";
    std::cout << "Effect: " << c->getEffect() << "\n\n";

    // 3) If the command is valid , run the actual tournament
    if (c->getEffect().rfind("Invalid", 0) == 0) {
        std::cout << "Tournament command was invalid, not running GameEngine::runTournament.\n";
        delete c;
        return;
    }

    GameEngine engine;

    std::vector<std::string> maps = {
        "canada.map",
        "europe.map"
    };

    std::vector<std::string> strategies = {
        "aggressive",
        "benevolent",
        "neutral"
    };

    int G = 3;   // games per map
    int D = 10;  // max turns per game

    engine.runTournament(maps, strategies, G, D);

    delete c;
}