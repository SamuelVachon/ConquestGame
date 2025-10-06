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
    std::list<std::string> edgesNames;
*/

Territory::Territory(std::string& tempName, int tempx, int tempy, Continent* tempContinent){
    this->name = tempName;
    this->x = tempx;
    this->y = tempy;
    this->isConnected = false;
    this->army = 0;
    this->continent = tempContinent;
    this->continent->addTerritory(this);
};

Territory::~Territory(){
    this->edges.clear();
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
void Territory::addEdges(Territory* territory){
    this->edges.push_back(territory);
}
void Territory::addEgdesNames(std::string& name){
    this->edgesNames.push_back(name);
}
void Territory::setTerritoriesEdges(Map* map){
    Territory* territory;
    for(std::string& edgesName : this->edgesNames){
        territory = map->getTerritory(edgesName);
        this->addEdges(territory);
    }
}
void Territory::setConnectedtoTrue(){
    std::stack<Territory*> connectedTerritories;
    Territory* territory;
    for(Territory*& i : (this->edges)){
        if(i->getIsConnected()){
            continue;
        }else{
            connectedTerritories.push(i);
            i->setIsConnected(true);
        }
    }
    while(!(connectedTerritories.empty())){
        territory = connectedTerritories.top();
        territory->setConnectedtoTrue();
        connectedTerritories.pop();
    }
}
void Territory::setConnectedtoTrue(Continent* continent){
    std::stack<Territory*> connectedTerritories;
    Territory* territory;
    for(Territory*& i : this->edges){
        if(i->getIsConnected()){
            continue;
        }else{
            if(continent->compare(i->continent)){
                connectedTerritories.push(i);
                i->setIsConnected(true);
            }
            else{
                continue;
            }
        }
    }
    while(!(connectedTerritories.empty())){
        territory = connectedTerritories.top();
        territory->setConnectedtoTrue(continent);
        connectedTerritories.pop();
    }
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
std::list<std::string> Territory::getEdgesNames(){
    return this->edgesNames;
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

std::string Territory::toString(){
    std::string separator = ", ";
    std::string edges;
    for(Territory*& i : this->edges){
        edges = edges + separator + i->getName();
    }
    return this->name + separator + std::to_string(this->x) + separator + std::to_string(this->y) + separator  + this->continent->getName() + separator + edges;
}

/*
Methods for class Continent, this is a class responsible to keep track which territory are in which continent. 
std::string name;
int number;
std::list<Territory*>* territories;
*/ 

Continent::Continent(std::string& tempName, int tempNumber){
    this->name = tempName;
    this->number = tempNumber;
}

Continent::~Continent(){
    this->territories.clear();
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

bool Continent::compare(Continent* continent){
    if(this->name.compare(continent->name)){
        return true;
    }
    return false;
}
bool Continent::validate(){
    Territory* firstTerritory = this->territories.front();
    firstTerritory->setConnectedtoTrue(this);
    for(Territory*& i : this->territories){
        if(i->getIsConnected()){
            i->setIsConnected(false);
            continue;
        }
        else{
            for(Territory*& i : this->territories){
                i->setIsConnected(false);
            }
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
std::list<Territory*> Continent::getTerritories(){
    return this->territories;
}

std::string Continent::toString(){
    std::string territories;
    std::string separator = " ,";
    for(Territory*& i : this->territories){
        territories = territories + separator + i->getName();
    }
    return this->name + separator + territories;
}




/*
Methods for class Map, this is the class responsible to store all the territories. It also act as a control class for them. 
    std::string author;
    std::string image;
    bool wrap;
    std::string scroll;
    bool warn;
    std::list<Territory*> territories;
    std::list<Continent*> continents;
*/ 

Map::Map(std::string& tempAuthor,std::string& tempImage, bool tempWrap,std::string& tempScroll, bool tempWarn){
    this->author = tempAuthor;
    this->image = tempImage;
    this->wrap = tempWrap;
    this->scroll = tempScroll;
    this->warn = tempWarn;
}

Map::~Map(){
    for(Territory*& i : this->territories){
        delete i;
    }
    for(Continent*& i : this->continents){
        delete i; 
    }
    this->territories.clear();
    this->continents.clear();

};

void Map::setAuthor(std::string& author){
    this->author = author;
}
void Map::setImage(std::string& image){
    this->image=image;
}
void Map::setWrap(bool wrap){
    this->wrap=wrap;
}
void Map::setScroll(std::string& scroll){
    this->scroll = scroll;
}
void Map::setWarn(bool warn){
    this->warn=warn;
}
void Map::addTerritory(Territory* territory){
    this->territories.push_back(territory);
}
void Map::addContinents(Continent* continent){
    this->continents.push_back(continent);
}


std::string Map::getAuthor(){
    return this->author;
}
std::string Map::getImage(){
    return this->image;
}
bool Map::getWrap(){
    return this->wrap;
}
std::string Map::getScroll(){
    return this->scroll;
}
bool Map::getWarn(){
    return this->warn;
}
std::list<Territory*> Map::getTerritories(){
    return this->territories;
}
std::list<Continent*> Map::getContinents(){
    return this->continents;
}

Territory* Map::getTerritory(std::string territoryName){
    std::string temp;
    for(Territory*& i : this->territories){
        temp = i->getName();
        if(temp.compare(territoryName)){
            return i;
        }
    }
    throw -1;
}

Continent* Map::getContinent(std::string continentName){
    std::string temp;
    for(Continent*& i : this->continents){
        temp = i->getName();
        if(temp.compare(continentName)){
            return i;
        }
    }
    throw -2;
}

void Map::setTerritoriesEdges(){
    for(Territory*& i : this->territories){
        i->setTerritoriesEdges(this);
    }
}

bool Map::validate(){
    Territory* firstTerritory = this->territories.front();
    firstTerritory->setConnectedtoTrue();
    for(Territory*& i : this->territories){
        if(i->getIsConnected()){
            i->setIsConnected(false);
            continue;
        }
        else{
            for(Territory*& i : this->territories){
                i->setIsConnected(false);
            }
            return false;
        }
    }
    for (Continent*& i : this->continents){
        if(i->validate()){
            continue;
        }
        else{
            return false;
        };
    }
    std::string separator = ", ";
    std::cout << this->toString() << std::endl;
    return true;
}

std::string Map::toString(){
    std::string separator = ", ";
    std::string continents;
    std::string territories;
    for(Territory*& i : this->territories){
        territories = territories+ (i->toString()) + "\n";
    }
    for(Continent*& i : this->continents){
        continents = continents+ (i->toString()) + "\n"; 
    }
    return "";
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
    std::ifstream inout;

    inout.open(mapFile);
    std::string line;
    std::string emptyLine;
    if(!std::getline(inout,line)){
        throw -30;
    }
    if(!line.compare("[Map]")){
        throw -31;
    }
   Map* map = this->loadMap(inout);

   if(!(std::getline(inout,emptyLine) && std::getline(inout,line))){
    throw -32;
   }
   if (emptyLine.compare("") && line.compare("[Continents]")){
    throw -33;
   }

   this->loadContinents(inout,map);

   if(!(std::getline(inout,line))){
    throw -34;
   }
   if(!line.compare("[Territories]")){
    throw -35;
   }

   this->loadTerritories(inout,map);

   inout.close();

   return map;

}

Map* MapLoader::loadMap(std::ifstream& inout){

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
    scroll = this->getValue(inout,"scroll");

    //load the warn value
    warn = this->getValue(inout,"warn");

    //Transfer string value yes and no to boolean value easier to handle afterward
    bool wrapBool;
    bool warnBool;

    if(wrap.compare("yes")){
        wrapBool=true;
    }
    else if(wrap.compare("no")){
        wrapBool=false;
    }
    else{
        throw -40;
    }

    if(warn.compare("yes")){
        warnBool=true;
    }
    else if(warn.compare("no")){
        warnBool=false;
    }
    else{
        throw -41;
    }
    Map* map = new Map(author,image,wrapBool,scroll,warnBool);
    return map;


}

std::string MapLoader::getValue(std::ifstream& inout, std::string type){
    std::string tempType;
    std::string line;
    std::string temp;
    char delimiter = '=';

    if(!(std::getline(inout,line))){
        throw -50;
    };
    std::istringstream ss(line);
    if(!(std::getline(ss,tempType,delimiter))){
        throw -51;
    }
    if(tempType.compare(type)){
        throw -52;
    }
    if(!(std::getline(ss,temp,delimiter))){
        throw -53;
    }
    std::cout << temp << std::endl;
    return temp;
}

void MapLoader::loadContinents(std::ifstream& inout, Map* map){
    std::string line;
    char delimiter = '=';
    std::string name;
    std::string temp;
    int number;

    while(std::getline(inout,line)){
        if(line.compare("\r") || line.compare("")){
            break;
        }
        std::istringstream ss(line);
        if(!std::getline(ss,name,delimiter)){
            throw -61;
        }
        if(!std::getline(ss,temp)){
            throw -62;
        }
        number = stoi(temp);
        this->continent = new Continent(name,number);
        map->addContinents(this->continent);
    }
}

void MapLoader::loadTerritories(std::ifstream& inout, Map* map){
    std::string line;
    char delimiter = ',';
    std::string temp;
    std::string name;
    std::string xString;
    std::string yString;
    int x;
    int y;
    std::string continentName;
    
        
    while(std::getline(inout,line)){
        if(line.compare("")){
            continue;
        }
        std::istringstream ss(line);
        if(!(std::getline(ss,name,delimiter))){
            throw -70;
        }
        if(!(std::getline(ss,xString,delimiter))){
            throw -71;
        }
        if(!(std::getline(ss,yString,delimiter))){
            throw -72;
        }
        if(!(std::getline(ss,continentName,delimiter))){
            throw -73;
        }
        this->continent = map->getContinent(continentName);
        x = stoi(xString);
        y = stoi(yString);
        this->territory = new Territory(name,x,y,this->continent);
        map->addTerritory(this->territory);
        while(std::getline(ss,temp,delimiter)){
            territory->addEgdesNames(temp);
        }
        if(!(ss.eof())){
            throw -74;
        }
    }
    if(!(inout.eof())){
        throw -75;
    }
    map->setTerritoriesEdges();
}





