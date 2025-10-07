#include "Map.h"
#include <iostream>

std::vector<Map*> testLoadMaps(std::vector<std::string> mapNames){
    MapLoader* mapLoader = new MapLoader();
    std::vector<Map*> maps;
    Map* map;
    for(std::string mapFile: mapNames){
        try{
            map = mapLoader->loadMap(mapFile);
            std::cout << "Map " << mapFile << " is succesfully loaded!" << std::endl;
            if(map->validate()){
                std::cout << "The map " << mapFile << " is valid!" << std::endl;
                maps.push_back(map);
            }
            else{
                std::cout << "The map " << mapFile << " is invalid!" << std::endl;
            }
        }
        catch(const int a){
        std::cout<<"Error with the file format. Code: "<< std::to_string(a) << std::endl;
        }
        catch(const std::runtime_error& e){
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    delete mapLoader;
    return maps;
}

int main(){

    std::vector<std::string> mapNames = {"./Maps/Earth.map", "./Maps/World.map", "Maps/InvalidMap.map", "Maps/InvalidMapSubGraph.map", "Maps/UncompatibleMap.map", "Maps/UncompatibleMap.txt"};
    std::vector<Map*> maps = testLoadMaps(mapNames);

    for(Map* map : maps){
        delete map;
    }
    return 0;
}