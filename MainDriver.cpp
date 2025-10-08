#include <iostream>
#include <cstdlib>
#include <vector>
#include "Player.h"
#include "Card.h"
#include "Orders.h"
#include "GameEngine.h"
#include "Map.h"

void test(){
    cout << "\n|||||||| Running test sequence of all drivers ||||||||" << endl;

    cout << "\n|||||||| MapDriver ||||||||\n\n" << endl;
    std::vector<std::string> mapNames = {"./Maps/Earth.map", "./Maps/World.map", "Maps/InvalidMap.map", "Maps/InvalidMapSubGraph.map", "Maps/UncompatibleMap.map", "Maps/UncompatibleMap.txt"};
    std::vector<Map*> maps = testLoadMaps(mapNames);

    for(Map* map : maps){
        delete map;
    }
    cout << "\n|||||||| Player Driver ||||||||\n\n" << endl;

    testPlayers();

    cout << "\n|||||||| OrderDriver ||||||||\n\n" << endl;

    testOrdersLists();

    cout << "\n|||||||| CardDriver ||||||||\n\n" << endl;
    testCards();

    cout << "\n|||||||| EngineDriver ||||||||\n\n" << endl;
    testGameStates();
}

int main(){
    test();
    return 0;
}
