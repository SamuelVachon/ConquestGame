//Header file for the declaration of the class Map and the Class Territories

#ifndef MAP_H
#define MAP_H

#include "Player.h"
#include <string>
#include <iostream>

Class Territory{
    public:
    void setName(std::string name);
    void setX(int x);
    void setY(int y);
    void setIsConnected(bool isConnected);
    void setArmy(int army);
    void setPlayer(Player* player);
    void addEdges(Territory* territory);
    
    std::string getName();
    int getX();
    int getY();
    Territory*[] getEdges();
    Continent* getContinent();
    bool getIsConnected();
    int getArmy();
    Player* getPlayer();


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

    void setName(std::string name);
    void setNumber(int number);
    void addTerritory(Territory* territory);

    std::string getName();
    int getNumber();
    Territory*[] getTerritories();

    private:

    std::string name;
    int number;
    Territory*[] territories;
}


Class Map{
    public:

    void setImage(std::string image);
    void setWrap(bool wrap);
    void setWarn(bool warn);
    void addTerritory(Territory* territory);

    std::string getImage();
    bool getWrap();
    bool getWarn();
    Territory*[] getTerritories();

    bool validate();

    private:

    std::string image;
    bool wrap;
    bool warn;
    Territory*[] territories;



}

Class MapLoader{
    public:

    void setDefaultMapFile(std::string mapFile);

    std::string getDefaultMapFile();

    Map* loadMap();
    
    Map* loadMap(std::string mapFile);

    private:
        
    std::string defaultMapFile;
}

#endif
