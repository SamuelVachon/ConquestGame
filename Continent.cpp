#include "Map.h"

/*
Methods for class Continent, this is a class responsible to keep track which territory are in which continent. 
std::string name;
int number;
std::vector<int> territoriesIndex; -> represent the index in the map object territories attribute
*/ 

Continent::Continent(std::string& tempName, int tempNumber){
    this->name = tempName;
    this->number = tempNumber;
}

Continent::~Continent(){
    this->territoriesIndex.clear();
};

void Continent::setName(std::string& name){
    this->name=name;
}
void Continent::setNumber(int number){
    this->number=number;
}
void Continent::addTerritory(int territory){
    this->territoriesIndex.push_back(territory);
}

bool Continent::compare(Continent* continent){
    if(this->name.compare(continent->name)){
        return true;
    }
    return false;
}

//Returne true if the Continent is a connected subgraph, return false otherwise
bool Continent::validate(Map* map, int index){
    int territory = this->territoriesIndex[0];
    map->getTerritories()[territory]->setIsConnected(true);
    map->getTerritories()[territory]->setConnectedtoTrue(map, index); 
    for(int terr : this->territoriesIndex){
        if(map->getTerritories()[terr]->getIsConnected()){
            map->getTerritories()[terr]->setIsConnected(false);
            continue;
        }
        else{
            std::cout << "The continent" << this->getName() <<  "is not a connected sub graph since " << map->getTerritories()[terr]->getName() << " is not connected to the other territories." << std::endl;
            return false;
        }
    }
    return true;
}

std::string Continent::getName(){
    return this->name;
}
int Continent::getNumber(){
    return this->number;
}
std::vector<int> Continent::getTerritories(){
    return this->territoriesIndex;
}

std::string Continent::toString(){
    return "";
}
