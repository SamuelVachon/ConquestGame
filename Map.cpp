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

Continent::Continent(Continent* continent){
    this->name = continent->name;
    this->number = continent->number;
    this->territoriesIndex = continent->territoriesIndex;
}

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

std::ostream& operator<<(std::ostream& os, const Continent& c){
    os << c.name;
    return os;
}



/*Methods for the class Territory, this class is responsible for storing the diiferent infos on each territory
    std::string name;
    int x;
    int y;
    std::vector<int> edgesIndex; -> represent the index in the map object territories attribute, possibility of improvement by using pointers
    int continentIndex; -> represent the index in the map object continents attribute
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

Territory::Territory(Territory* territory){
    this->name = territory->name;
    this->x = territory->x;
    this->y = territory->y;
    this->isConnected = territory->isConnected;
    this->army = territory->army;
    this->continentIndex = territory->continentIndex;
    this->edgesIndex = territory->edgesIndex;
    this->player = territory->player;
}

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
void Territory::addEdgesNames(std::string& name){
    this->edgesNames.push_back(name);
}

//Helper method for the validate function of the Class map, this function set the value of
//the isConnected attribute to true for all territories that can be reached stating from
//the object instance on which it is called onto
void Territory::setConnectedtoTrue(Map* map){
    std::stack<int> connectedTerritories;
    int temp;
    for(int edge : this->edgesIndex){
        if(map->getTerritories()[edge]->getIsConnected()){
            continue;
        }
        else{
            map->getTerritories()[edge]->setIsConnected(true);
            connectedTerritories.push(edge);
        }
    }
    while(!connectedTerritories.empty()){
        temp = connectedTerritories.top();
        map->getTerritories()[temp]->setConnectedtoTrue(map);
        connectedTerritories.pop();
    }
}
//Helper method for the validate function of the Class continent, this function set the value of
//the isConnected attribute to true for all territories that can be reached stating from
//the object instance on which it is called onto if it is part of the same continent as iy
void Territory::setConnectedtoTrue(Map* map,int continentIndex){
    std::stack<int> connectedTerritories;
    int temp;
    for(int edge : this->edgesIndex){
        if(map->getTerritories()[edge]->getIsConnected()){
            continue;
        }
        else{
            if((map->getTerritories()[edge]->getContinent()) == continentIndex){
                map->getTerritories()[edge]->setIsConnected(true);
                connectedTerritories.push(edge);
            }
            else{
                continue;
            }
        }
    }
    while(!connectedTerritories.empty()){
        temp = connectedTerritories.top();
        map->getTerritories()[temp]->setConnectedtoTrue(map, continentIndex);
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

std::ostream& operator<<(std::ostream& os,const Territory& t){
    os << t.name << " Armies: "<< t.army <<" Coordinates: [" << t.x << "," << t.y << "]";
    return os;
}


/*
Methods for class Map, this is the class responsible to store all the territories. It also act as a control class for them.
    std::string author;
    std::string image;
    bool wrap;
    std::string scroll;
    bool warn;
    std::vector<Territory*> territories;
    std::vector<Continent*> continents;
*/

Map::Map(std::string& tempAuthor,std::string& tempImage, bool tempWrap,std::string& tempScroll, bool tempWarn){
    this->author = tempAuthor;
    this->image = tempImage;
    this->wrap = tempWrap;
    this->scroll = tempScroll;
    this->warn = tempWarn;
}

Map::Map(Map* map){
    this->author = map->author;
    this->image = map->image;
    this->wrap = map->wrap;
    this->scroll = map->scroll;
    this->warn = map->warn;
    for(Territory* terr : map->territories){
        this->addTerritory(new Territory(terr));
    }
    for(Continent* cont : map->continents){
        this->addContinents(new Continent(cont));
    }
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

//Return the index fot the territories atribute based on the name of the territory to find
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

//Return the index for the continents attribute based on the name of the continent to find
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

//Helper function for the loadMap() function, Since the file only contains the name of the
//territories that borders them, this function find the index of each edges and add it to the edges attribute of the territory
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

//Helper function for the loadMap() function, the territories already holds the continent
//This function add each territory index that are part of a continent to said continent attribute territories
void Map::setContinentsTerritories(){
    int size = this->territories.size();
    for(int i=0;i<size;i++){
        this->continents[this->territories[i]->getContinent()]->addTerritory(i);
    }
}

//Return true if the map is a connected graph anf if all continents are connected subgraph
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

std::ostream& operator<<(std::ostream& os,const Map& m){
    os << "Author: " << m.author << "\n\n";
    os << "Continents:\n";
    for (Continent* cont : m.continents){
        os << *cont << "\n";
    }
    os << "\nTerritories:\n";
    for(Territory* terr : m.territories){
        os << *terr << "\n";
    }
    return os;
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
//Throw a negative number if the file is invalid, Posibility of improvement by throwing error messages instead
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
            territory->addEdgesNames(temp);
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





