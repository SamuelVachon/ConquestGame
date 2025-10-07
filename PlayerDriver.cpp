// PlayerDriver.cpp
#include <iostream>
#include <vector>
#include "Player.h"
#include "Card.h"           // Hand, Card, CardType
#include "Orders.h"

// include teammate's Map header (note the space in the filename)
#include "Map.h"

// Free function required by the rubric
void testPlayers() {
    std::cout << "=== testPlayers() ===\n";

    // Build a tiny continent + a few territories using your teammate's API
    MapLoader* loader = new MapLoader();
    std::string mapFile = "./Maps/Earth.map";
    Map* map = loader->loadMap(mapFile);
    delete loader;
    
    std::vector<Continent*> continents = map->getContinents();
    std::vector<Territory*> territories = map->getTerritories();

    Territory* t1 = territories[continents[0]->getTerritories()[0]];
    Territory* t2 = territories[continents[0]->getTerritories()[1]];
    Territory* t3 = territories[continents[0]->getTerritories()[2]];

    // Player with territories, hand, and orders list
    Player p("Alice");
    p.addTerritory(t1);
    p.addTerritory(t2);
    p.addTerritory(t3);

    // Give the player a Hand using the real Card API
    Hand* h = new Hand();
    h->addCard(new Card(CardType::Bomb));
    h->addCard(new Card(CardType::Airlift));
    p.setHand(h);

    // Show summary
    std::cout << p << "\n";

    // Demonstrate toDefend / toAttack returning Territory lists
    auto defend = p.toDefend();
    auto attack = p.toAttack();

    std::cout << "toDefend: ";
    for (auto* t : defend) std::cout << t->getName() << " ";
    std::cout << "\n";

    std::cout << "toAttack: ";
    for (auto* t : attack) std::cout << t->getName() << " ";
    std::cout << "\n";

    // Demonstrate issueOrder placing into OrdersList
    p.issueOrder();
    p.issueOrder();
    std::cout << "Orders after issuing: " << *p.getOrders() << "\n";

    std::cout << "=== end testPlayers() ===\n";
    delete map;
}
