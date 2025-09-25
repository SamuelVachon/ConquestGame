//Header file for the declaration of the class Map and the Class Territories

#ifndef MAP_H
#define MAP_H

#include "Player.h"
#include <string>
#include <iostream>

Class Territory{
    public:

    private:

    std::string name;
    int x;
    int y;
    Territory*[] edges;
    Continent* continent;
    bool isConnected;
    int army;
    Player* player;
    
}

Class Continent{

    public:



    private:

    std::string name;
    int number;
    Territory*[] territories;
}


Class Map{
    public:

    bool validate();

    private:

    std::string image;
    bool wrap;
    bool warn;
    Territory*[] territories;



}

Class MapLoader{
    public:

    Map* loadMap(std::string mapFile);

    private:
        
    std::string defaultMapFile;
}






#endif
