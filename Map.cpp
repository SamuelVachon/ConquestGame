#include "Map.h"


/*Methods for the class Territory, this class is responsible for storing the diiferent infos on each territory
    std::string name;
    int x;
    int y;
    std::list<Territory*> edges;
    Continent* continent;
    bool isConnected;
    int army;
    Player* player;
*/


void Territory::setName(std::string name){
    this->name = name;
}
void Territory::setX(int x){
    this->x=x;
}
void Territory::setY(int y){
    this->y = y;
}
void Territory::setIsConnected(bool isConnected){
    this->isConnected = isConnected;
}
void Territory::setArmy(int army){
    this->army=army;
}
void Territory::setPlayer(Player* player){
    this->player=player;
}
void addEdges(Territory* territory){
    this->edges.insert(territory);
}
   
std::string Territory::getName(){
    return this->name;
}
int Territory::getX(){
    return this->x;
}
int Territory::getY(){
    return this->y;
}
std::list<Territories*> Territory::getEdges(){
    return this->edges;
}
Continent* Territory::getContinent(){
    return this->continent;
}
bool Territory::getIsConnected(){
    return this->isConnected;
}
int Territory::getArmy(){
    return this->army;
}
Player* Territory::getPlayer(){
    return this->player;
}

/*
Methods for class Continent, this is a class responsible to keep track which territory are in which continent. 
std::string name;
int number;
std::list<Territory*> territories;
*/ 


void Continent::setName(std::string name){
    this->name=name;
}
void Continent::setNumber(int number){
    this->number=number;
}
void Continent::addTerritory(Territory* territory){
    this->territories.insert(territory);
}
std::string Continent::getName(){
    return this->name;
}
int Continent::getNumber(){
    return this->number;
}
std::list<Territory*> Continent::getTerritories(){
    return this->territories;
}



/*
Methods for class Map, this is the class responsible to store all the territories. It also act as a control class for them. 
std::string image
bool wrap
bool warn
std::list<Territory*> territories
*/ 

void Map::setImage(std::string image){
    this->image=image;
}
void Map::setWrap(bool wrap){
    this->wrap=wrap;
}
void Map::setWarn(bool warn){
    this->warn=warn;
}
void Map::addTerritory(Territory* territory){
    this->territories.insert(territory);
}

std::string Map::getImage(){
    return this->image;
}
bool Map::getWrap(){
    return this->wrap;
}
bool Map::getWarn(){
    return this->warn;
}
std::list<Territory*> Map::getTerritories(){
    return this->territories;
}

bool Map::validate(){

}


/*
Methods for class MapLoader, this is the class responsible to load the map at the start of each game.
It also stores a default map. 
std::string defaultMapFile
*/ 

void MapLoader::setDefaultMapFile(std::string mapFile){
    this->defaultMapFile=mapFile;
}

std::string MapLoader::getDefaultMapFile(){
    return this->defaultMapFile;
}

Map* MapLoader::loadMap(){
    this->loadMap(defaultMapFile);
}
    
Map* MapLoader::loadMap(std::string mapFile){

}

