//Header file for the declaration of the class Map and the Class Territories

#ifndef MAP_H
#define MAP_H

//#include "Player.h"
#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>

class Player;
class Territory;

class Continent{

    public:

    Continent(std::string& name, int number);
    ~Continent();
    void setName(std::string& name);
    void setNumber(int number);
    void addTerritory(Territory* territory);

    std::string getName();
    int getNumber();
    std::list<Territory*> getTerritories();

    private:

    std::string name;
    int number;
    std::list<Territory*> territories;
};

class Territory{
    public:

    Territory(std::string& name, int x, int y);
    ~Territory();
    void setName(std::string& name);
    void setX(int x);
    void setY(int y);
    void setIsConnected(bool isConnected);
    void setArmy(int army);
    void setPlayer(Player* player);
    void addEdges(Territory* territory);
    
    std::string getName();
    int getX();
    int getY();
    std::list<Territory*> getEdges();
    Continent* getContinent();
    bool getIsConnected();
    int getArmy();
    Player* getPlayer();


    private:

    std::string name;
    int x;
    int y;
    std::list<Territory*> edges;
    Continent* continent;
    bool isConnected;
    int army;
    Player* player;
    
};


class Map{
    public:

    Map(std::string& author,std::string& image, bool wrap,std::string& scroll, bool warn);
    ~Map();
    void setImage(std::string& image);
    void setWrap(bool wrap);
    void setWarn(bool warn);
    void addTerritory(Territory* territory);
    void addContinents(Continent* continent);

    std::string getImage();
    bool getWrap();
    bool getWarn();
    std::list<Territory*> getTerritories();
    std::list<Continent*> getContinents();

    bool validate();

    private:

    std::string author;
    std::string image;
    bool wrap;
    std::string scroll;
    bool warn;
    std::list<Territory*> territories;
    std::list<Continent*> continents;
};

class MapLoader{
    public:

    MapLoader();
    MapLoader(std::string& mapFile);
    ~MapLoader();
    void setDefaultMapFile(std::string& mapFile);
    std::string getDefaultMapFile();
    Map* loadMap();
    Map* loadMap(std::string& mapFile);
    

    private:
  
    std::string defaultMapFile;
    Map* loadMap(std::fstream& inout);
    std::string getValue(std::fstream& inout, std::string& type)
};

#endif
