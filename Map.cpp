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

Territory::Territory(std::string& name, int x, int y){
    this->name = name;
    this->x = x;
    this->y = y;
    this->isConnected = false;
    this->army = 0;
};

Territory::~Territory(){
    
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
void Territory::addEdges(Territory* territory){
    this->edges.push_back(territory);
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
std::list<Territory*> Territory::getEdges(){
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

Continent::Continent(std::string& name, int number){
    this->name = name;
    this->number = number;
}

Continent::~Continent(){
    
};

void Continent::setName(std::string& name){
    this->name=name;
}
void Continent::setNumber(int number){
    this->number=number;
}
void Continent::addTerritory(Territory* territory){
    this->territories.push_back(territory);
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

Map::Map(std::string& author,std::string& image, bool wrap,std::string& scroll, bool warn){
    this->author = author;
    this->image = image;
    this->wrap = wrap;
    this->scroll = scroll;
    this->warn = warn;
}

Map::~Map(){
    
};

void Map::setImage(std::string& image){
    this->image=image;
}
void Map::setWrap(bool wrap){
    this->wrap=wrap;
}
void Map::setWarn(bool warn){
    this->warn=warn;
}
void Map::addTerritory(Territory* territory){
    this->territories.push_back(territory);
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

MapLoader::MapLoader(){

};

MapLoader::MapLoader(std::string& mapFile){
    this->defaultMapFile = mapFile;
}

MapLoader::~MapLoader(){
    
};

void MapLoader::setDefaultMapFile(std::string& mapFile){
    this->defaultMapFile=mapFile;
}

std::string MapLoader::getDefaultMapFile(){
    return this->defaultMapFile;
}

Map* MapLoader::loadMap(){
    return this->loadMap(this->defaultMapFile);
}
    
//Take the name of a map file and load all the informations into a map object
//Throw -1 if the file is invalid
Map* MapLoader::loadMap(std::string& mapFile){
    std::fstream inout;

    inout.open(mapFile, ios::in);
    std::string line;
    std::string emptyLine;
    if(!(getLine(inout,line))){
        throw -1;
    }
    if(line != "[Map]"){
        throw -1;
    }
   Map* map = this->loadMap(inout);

   if(!(getLine(inout,emptyLine) || !(getLine(inout,line)))){
    throw -1;
   }
   if (emptyLine!="" || line!="[Continents]"){
    throw -1;
   }

   this->loadContinents(inout,map);

   if(!(getLine(inout,line))){
    throw -1;
   }
   if(line!="[Territories]"){
    throw -1;
   }

   this->loadTerritories(inout,map);

}

Map* MapLoader::loadMap(std::fstream& inout){

    std::string author;
    std::string image;
    std::string wrap;
    std::string scroll;
    std::string warn;


    //Load the author name
    author = this->getValue(inout,"author");
    
    //load the image file name
    image = this->getValue(inout,"image");

    //load the wrap value
    wrap = this->getValue(inout,"wrap");

    //load the scroll value
    scroll = this->getValue(inout,"scroll")

    //load the warn value
    warn = this->getValue(inout,"warn");

    //Transfer string value yes and no to boolean value easier to handle afterward
    bool wrapBool;
    bool warnBool;

    if(wrap=="yes"){
        wrapBool=true;
    }
    else if(wrap=="no"){
        wrapBool=false;
    }
    else{
        throw -1;
    }

    if(warn=="yes"){
        warnBool=true;
    }
    else if(warn=="no"){
        warnBool=false;
    }
    else{
        throw -1;
    }

    return new Map(author,image,wrapBool,scroll,warnBool);


}

std::string MapLoader::getValue(std::fstream& inout, std::string& type){
    std::string temp;
    std::string line;
    std::string delimiter = "=";

    if(!(getLine(inout,line))){
        throw -1;
    };
    std::istringstream ss(line);
    if(!(getLine(ss,temp,delimiter))){
        throw -1;
    };
    if(temp != type){
        throw -1;
    }
    if(!(getLine(ss,temp))){
        throw -1;
    }
    return temp;
}

void MapLoader::loadContinents(std::fstream& inout, Map* map){
    std::string line;
    std::string delimiter = "=";
    std::string name;
    std:string temp;
    int number;

    while(true){
        if(!(getLine(inout,line))){
            throw -1;
        };
        if(line==""){
            break;
        }
        std::istringstream ss(line);
        if(!getLine(ss,name,delimiter)){
            throw -1;
        }
        if(!getLine(ss,temp)){
            throw -1;
        }
        number = stoi(temp);
        Continent* continent = new Continent(name,number);
        map->addContinents(continent);
    }

void MapLoader::loadTerritories(std::fstream& inout, Map* map){
    std::string line;
    std::string delimiter = ",";
    
}


}

