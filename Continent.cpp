#include "Map.h"

/*
Methods for class Continent, this is a class responsible to keep track which territory are in which continent. 
std::string name;
int number;
std::list<int> territories;
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
bool Continent::validate(){
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