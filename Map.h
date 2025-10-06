//Header file for the declaration of the class Map and the Class Territories

#ifndef MAP_H
#define MAP_H

//#include "Player.h"
#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <stack>

class Player;
class Territory;
class Map;

class Continent{

    public:

    Continent(std::string& tempName, int tempNumber);
    ~Continent();
    void setName(std::string& name);
    void setNumber(int number);
    void addTerritory(Territory* territory);
    bool compare(Continent* continent);
    bool validate();

    std::string getName();
    int getNumber();
    std::list<Territory*> getTerritories();

    std::string toString();

    private:

    std::string name;
    int number;
    std::list<Territory*> territories;
};

class Territory{
    public:

    Territory(std::string& tempName, int tempx, int tempy, Continent* tempContinent);
    ~Territory();
    void setName(std::string& name);
    void setX(int x);
    void setY(int y);
    void setIsConnected(bool isConnected);
    void setArmy(int army);
    void setPlayer(Player* player);
    void addEdges(Territory* territory);
    void addEgdesNames(std::string& name);
    void setTerritoriesEdges(Map* map);
    void setConnectedtoTrue();
    void setConnectedtoTrue(Continent* continent);

    std::string getName();
    int getX();
    int getY();
    std::list<Territory*> getEdges();
    std::list<std::string> getEdgesNames();
    Continent* getContinent();
    bool getIsConnected();
    int getArmy();
    Player* getPlayer();

    std::string toString();
    


    private:

    std::string name;
    int x;
    int y;
    std::list<Territory*> edges;
    Continent* continent;
    bool isConnected;
    int army;
    Player* player;
    std::list<std::string> edgesNames;
    
};


class Map{
    public:

    Map(std::string& tempAuthor,std::string& tempImage, bool tempWrap,std::string& tempScroll, bool tempWarn);
    ~Map();
    void setAuthor(std::string& author);
    void setImage(std::string& image);
    void setWrap(bool wrap);
    void setScroll(std::string& scroll);
    void setWarn(bool warn);
    void addTerritory(Territory* territory);
    void addContinents(Continent* continent);

    std::string getAuthor();
    std::string getImage();
    bool getWrap();
    std::string getScroll();
    bool getWarn();
    std::list<Territory*> getTerritories();
    std::list<Continent*> getContinents();
    Territory* getTerritory(std::string territoryName);
    Continent* getContinent(std::string continentName);
    void setTerritoriesEdges();

    bool validate();

    std::string toString();

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
  
    Map* loadMap(std::ifstream& inout);
    std::string getValue(std::ifstream& inout, std::string type);
    void loadContinents(std::ifstream& inout, Map* map);
    void loadTerritories(std::ifstream& inout, Map* map);
    std::string defaultMapFile;
    Territory* territory;
    Continent* continent;
    
    
};

#endif
