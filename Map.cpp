#include "Map.h"


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
    for(Territory* i : this->territories){
        delete i;
    }
    for(Continent* i : this->continents){
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
std::vector<Territory*> Map::getTerritories(){
    return this->territories;
}
std::vector<Continent*> Map::getContinents(){
    return this->continents;
}

int Map::getTerritory(std::string territoryName){
    std::string temp;
    int size=this->territories.size();
    for(int i=0;i<size;i++){
        temp = (this->territories[i])->getName();
        if(temp.compare(territoryName)){
            return i;
        }
    }
    throw -1;
}

int Map::getContinent(std::string continentName){
    std::string temp;
    int size = this->continents.size();
    for(int i=0;i<size;i++){
        temp = this->continents[i]->getName();
        if(temp.compare(continentName) == 0){
            return i;
        }
    }
    throw -2;
}

void Map::setTerritoriesEdges(){
    std::vector<int> edgesIndex;
    std::vector<std::string> edgesNames;
    std::string tempName;
    for(Territory* territory : this->territories){
        edgesNames = territory->getEdgesNames();
        for(std::string name : edgesNames){
            int size = territories.size();
            for(int i=0;i<size;i++){
                tempName = territories[i]->getName();
                if(name.compare(tempName) == 0){
                    territory->addEdges(i);
                    break;
                }
            }
        }
    }
}
void Map::setContinentsTerritories(){
    int size = this->territories.size();
    for(int i=0;i<size;i++){
        this->continents[this->territories[i]->getContinent()]->addTerritory(i);
    }
}

bool Map::validate(){
    Territory* territory = this->territories[0];
    territory->setIsConnected(true);
    territory->setConnectedtoTrue(this);
    for(Territory* terr : this->territories){
        if(terr->getIsConnected()){
            terr->setIsConnected(false);
            continue;
        }
        else{
            std::cout << "The map is not a connected graph since " << terr->getName() << " is not connected to the other territories." << std::endl;
            return false;
        }
    }
    Continent* continent;
    int size = this->continents.size();
    for(int i=0;i<size;i++){
        continent = this->continents[i];
        if(continent->validate(this, i)){
            continue;
        }
        else{
            return false;
        }
    }
    return true;
}

std::string Map::toString(){
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

bool MapLoader::getline(std::ifstream& inout, std::string& line){
    if(!std::getline(inout,line)){
        return false;
    }
    if(line.empty()){
        std::cout << line << std::endl;
        return true;
    }
    if(line.back() == '\r' || line.back() == '\n'){
        line.pop_back();
    }
    //std::cout << line << std::endl;
    return true;
}

bool MapLoader::getline(std::istringstream& ss, std::string& line, char delimiter){
    if(!(std::getline(ss,line,delimiter))){
        return false;
    }
    if(line.empty()){
        std::cout << line << std::endl;
        return true;
    }
    if(line.back() == '\r' || line.back() == '\n'){
        line.pop_back();
    }
    //std::cout << line << std::endl;
    return true;
}
    
//Take the name of a map file and load all the informations into a map object
//Throw -1 if the file is invalid
Map* MapLoader::loadMap(std::string& mapFile){
    std::ifstream inout(mapFile);

    std::string line;
    std::string emptyLine;
    std::string toCompare;
    if(!this->getline(inout,line)){
        throw -30;
    }
    toCompare = "[Map]";
    if(line.compare(toCompare) != 0){
        throw -31;
    }
   Map* map = this->loadMap(inout);

    if(!(this->getline(inout,emptyLine))){
        throw -321;
    }
    if(!(this->getline(inout,line))){
        throw -322;
    }
    
    toCompare = "[Continents]";
    if(line.compare(toCompare) != 0){
        throw -33;
    }

   this->loadContinents(inout,map);

   if(!(this->getline(inout,line))){
    throw -34;
   }
   toCompare = "[Territories]";
   if(line.compare(toCompare) != 0){
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

    if(!(this->getline(inout,line))){
        throw -50;
    };
    std::istringstream ss(line);
    if(!(this->getline(ss,tempType,delimiter))){
        throw -51;
    }
    if(tempType.compare(type)){
        throw -52;
    }
    if(!(this->getline(ss,temp,delimiter))){
        throw -53;
    }
    return temp;
}

void MapLoader::loadContinents(std::ifstream& inout, Map* map){
    std::string line;
    char delimiter = '=';
    std::string name;
    std::string temp;
    int number;
    std::istringstream ss;

    while(this->getline(inout,line)){
        if(line.empty()){
            break;
        }
        ss.clear();
        ss.str(line);
        if(!this->getline(ss,name,delimiter)){
            throw -61;
        }
        if(!this->getline(ss,temp,delimiter)){
            throw -62;
        }
        number = stoi(temp);
        Continent* continent = new Continent(name,number);
        map->addContinents(continent);
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
    std::istringstream ss;
        
    while(this->getline(inout,line)){
        if(line.empty()){
            continue;
        }
        ss.clear();
        ss.str(line);
        if(!(this->getline(ss,name,delimiter))){
            throw -70;
        }
        if(!(this->getline(ss,xString,delimiter))){
            throw std::runtime_error("Error in the reading of the file at line 350"+xString);;
        }
        if(!(this->getline(ss,yString,delimiter))){
            throw -72;
        }
        if(!(this->getline(ss,continentName,delimiter))){
            throw -73;
        }
        int continent = map->getContinent(continentName);
        x = stoi(xString);
        y = stoi(yString);
        Territory* territory = new Territory(name,x,y,continent);
        while(this->getline(ss,temp,delimiter)){
            territory->addEgdesNames(temp);
        }
        if(!(ss.eof())){
            throw -74;
        }
        map->addTerritory(territory);
    }
    if(!(inout.eof())){
        throw -75;
    }
    map->setTerritoriesEdges();
    map->setContinentsTerritories();
}





