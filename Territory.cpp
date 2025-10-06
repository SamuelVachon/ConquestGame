#include "Map.h"


/*Methods for the class Territory, this class is responsible for storing the diiferent infos on each territory
    std::string name;
    int x;
    int y;
    std::vector<int> edgesIndex;
    int continentIndex;
    bool isConnected;
    int army;
    Player* player;
    std::vector<std::string> edgesNames;
*/

Territory::Territory(std::string& tempName, int tempx, int tempy, int tempContinent){
    this->name = tempName;
    this->x = tempx;
    this->y = tempy;
    this->isConnected = false;
    this->army = 0;
    this->continent = tempContinent;
};

Territory::~Territory(){
    this->edgesIndex.clear();
    this->edgesNames.clear();
};

void Territory::setName(std::string& name){
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
void Territory::addEdges(int i){
    this->edgesIndex.push_back(i);
}
void Territory::addEgdesNames(std::string& name){
    this->edgesNames.push_back(name);
}

void Territory::setConnectedtoTrue(){
    
}
void Territory::setConnectedtoTrue(int continentIndex){
    
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
std::vector<int> Territory::getEdges(){
    return this->edgesIndex;
}
std::vector<std::string> Territory::getEdgesNames(){
    return this->edgesNames;
}
int Territory::getContinent(){
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

std::string Territory::toString(){
    return "";
}


