#include "Map.h"
#include <iostream>

int main(){
    std::string fileName;
    std::cout << "Enter the files path: ";
    std::cin >> fileName;
    std::cout << "\n" << std::endl;

    MapLoader* mapLoader = new MapLoader();
    Map* map;
    try{
        map = mapLoader->loadMap(fileName);
        std::cout << "Map successfully loaded!" << std::endl;
        if (map->validate()){
            std::cout << "The map is valid!" << std::endl;
        }
        else{
            std::cout << "The map is invalid!" << std::endl;
        }
    }
    catch(const int a){
        std::cout<<"Error with the file format."<< std::to_string(a) << std::endl;
        return 0;
    }
    catch(const std::runtime_error& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    //std::string tempCont = "North America";
    //Continent* continent = map->getContinent(tempCont);
    //std::string result = continent->getName();
    //std::cout << result << std::endl;
    std::cout << map->getAuthor() << std::endl;
    
    delete map;
    delete mapLoader;
    return 0;
}