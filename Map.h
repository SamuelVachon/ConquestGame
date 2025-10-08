//Header file for the declaration of the class Map and the Class Territories

#ifndef MAP_H
#define MAP_H

//#include "Player.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>

class Player;
class Territory;
class Map;

std::vector<Map*> testLoadMaps(std::vector<std::string> mapNames);

class Continent{

    public:

    Continent(std::string& tempName, int tempNumber);
    ~Continent();
    void setName(std::string& name);
    void setNumber(int number);
    void addTerritory(int territory);
    bool compare(Continent* continent);
    bool validate(Map* map, int index);


    std::string getName();
    int getNumber();
    std::vector<int> getTerritories();

    std::string toString();

    private:

    std::string name;
    int number;
    std::vector<int> territoriesIndex;
};

class Territory{
    public:

    Territory(std::string& tempName, int tempx, int tempy, int tempContinent);
    ~Territory();
    void setName(std::string& name);
    void setX(int x);
    void setY(int y);
    void setIsConnected(bool isConnected);
    void setArmy(int army);
    void setPlayer(Player* player);
    void addEdges(int i);
    void addEdgesNames(std::string& name);
    void setConnectedtoTrue(Map* map);
    void setConnectedtoTrue(Map* map, int continentIndex);

    std::string getName();
    int getX();
    int getY();

    std::vector<int> getEdges();
    std::vector<std::string> getEdgesNames();
    int getContinent();
    bool getIsConnected();
    int getArmy();
    Player* getPlayer();

    std::string toString();



    private:

    std::string name;
    int x;
    int y;
    std::vector<int> edgesIndex;
    int continentIndex;
    bool isConnected;
    int army;
    int continent;
    Player* player;
    std::vector<std::string> edgesNames;

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
    std::vector<Territory*> getTerritories();
    std::vector<Continent*> getContinents();
    int getTerritory(std::string territoryName);
    int getContinent(std::string continentName);
    void setTerritoriesEdges();
    void setContinentsTerritories();

    bool validate();

    std::string toString();

    private:

    std::string author;
    std::string image;
    bool wrap;
    std::string scroll;
    bool warn;
    std::vector<Territory*> territories;
    std::vector<Continent*> continents;
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

    bool getline(std::ifstream& inout, std::string& line);
    bool getline(std::istringstream& ss, std::string& line, char delimiter);
    Map* loadMap(std::ifstream& inout);
    std::string getValue(std::ifstream& inout, std::string type);
    void loadContinents(std::ifstream& inout, Map* map);
    void loadTerritories(std::ifstream& inout, Map* map);
    std::string defaultMapFile;
};

#endif
