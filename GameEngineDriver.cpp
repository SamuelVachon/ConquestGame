#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Card.h"
#include <iostream>

void testGameStates() {
    GameEngine engine;
    std::cout << "=== testGameStates() ===\n";
    std::cout << engine << "\n";
    engine.runConsole();
}


/**
 * testMainGameLoop()
 * Demonstrates:
 * (1) Reinforcement calculation
 * (2) Deploy orders until reinforcement pool = 0
 * (3) Advance orders (attack/defend)
 * (4) Playing cards to issue orders
 * (5) Player removal when no territories left
 * (6) Game ends when one player controls all territories
 */
void testMainGameLoop() {
    std::cout << "\n==============================\n";
    std::cout << " TESTING MAIN GAME LOOP DRIVER \n";
    std::cout << "==============================\n";

    // ====== SETUP MAP AND TERRITORIES ======
    std::string author = "Tester";
    std::string image = "none.bmp";
    std::string scroll = "none";
    std::string warn = "false";
    Map* map = new Map(author, image, false, scroll, false);

    std::string c1 = "North";
    std::string c2 = "South";
    Continent* north = new Continent(c1, 5);
    Continent* south = new Continent(c1, 3);
    map->addContinents(north);
    map->addContinents(south);

    std::string a="A", b="B", c="C", d="D";
    Territory* A = new Territory(a, 0, 0, 0);
    Territory* B = new Territory(b, 1, 0, 0);
    Territory* C = new Territory(c, 0, 1, 1);
    Territory* D = new Territory(d, 1, 1, 1);

    // Adjacency setup: A–B–C–D chain
    A->addAdjacentTerritory(B);
    A->addAdjacentTerritory(C);
    B->addAdjacentTerritory(A);
    B->addAdjacentTerritory(C);
    C->addAdjacentTerritory(B);
    C->addAdjacentTerritory(A);
    C->addAdjacentTerritory(D);
    D->addAdjacentTerritory(C);

    map->addTerritory(A);
    map->addTerritory(B);
    map->addTerritory(C);
    map->addTerritory(D);
    map->setContinentsTerritories();

    // ====== CREATE PLAYERS ======
    Player* p1 = new Player("Player 1");
    Player* p2 = new Player("Player 2");
    p1->setHand(new Hand());
    p2->setHand(new Hand());

    // Assign initial territories
    A->setPlayer(p1);
    B->setPlayer(p1);
    C->setPlayer(p2);
    D->setPlayer(p2);

    p1->addTerritory(A);
    p1->addTerritory(B);
    p2->addTerritory(C);
    p2->addTerritory(D);

    // ====== SETUP ENGINE AND DECK ======
    GameEngine* engine = new GameEngine();
    Deck* deck = new Deck(6);
    engine->map_ = map;
    engine->deck_ = deck;
    engine->players_ = new std::vector<Player*>{p1, p2};

    std::cout << "\n--- INITIAL SETUP COMPLETE ---\n";

    // ====== (1) REINFORCEMENT PHASE ======
    std::cout << "\n(1) Testing Reinforcement Phase...\n";
    std::cout << "Players receive reinforcements based on territory count and continent control.\n";
    std::cout << "Players start out with 50 reinforcements but for this, we will set it to 10\n";
    p1->setReinforcements(10);
    p2->setReinforcements(10);

    std::cout << p1->getName() << " reinforcement pool: " << p1->reinforcementPool() << std::endl;
    std::cout << p2->getName() << " reinforcement pool: " << p2->reinforcementPool() << std::endl;

    engine->reinforcementPhase();

    std::cout << p1->getName() << " reinforcement pool: " << p1->reinforcementPool() << std::endl;
    std::cout << p2->getName() << " reinforcement pool: " << p2->reinforcementPool() << std::endl;

    // ====== (2) ISSUE DEPLOY ONLY (WHILE HAVING ARMIES) ======
    std::cout << "\n(2) Testing Issue Orders Phase (Deploy Only)...\n";
    engine->issueOrdersPhase();

    std::cout << "\nOrders currently issued by each player:\n";
    std::cout << p1->getName() << ": " << p1->getOrders()->size() << " orders\n";
    std::cout << p2->getName() << ": " << p2->getOrders()->size() << " orders\n";

    std::cout << p1->getName() << " reinforcement pool: " << p1->reinforcementPool() << std::endl;
    std::cout << p2->getName() << " reinforcement pool: " << p2->reinforcementPool() << std::endl;

    // ====== (3) ADVANCE ORDERS (ATTACK / DEFEND) ======
    std::cout << "\n(3) Testing Issue Orders Phase (Advance Orders)...\n";
    std::cout << "Players will now issue Advance orders to defend or attack.\n";
    engine->issueOrdersPhase();

    std::cout << "\n(4) Testing Executing orders...\n";
    engine->executeOrdersPhase();
    // ====== (4) CARD PLAY ======
    std::cout << "\n(5) Testing Card Play...\n";
    p1->getHand()->addCard(deck->draw());
    p1->getHand()->addCard(deck->draw());
    std::cout << p1->getName() << " plays a card to issue an order.\n";
    p1->getHand()->playCard(0, p1, deck);

    // ====== (5) PLAYER ELIMINATION ======
    std::cout << "\n(6) Testing Player Elimination...\n";
    std::cout << "Simulating Player 2 losing all territories...\n";
    p2->getTerritories()->clear();

    std::cout << "\nRunning executing orders again...\n";
    engine->executeOrdersPhase();

    std::cout << "Remaining players: " << engine->players_->size() << std::endl;

    // ====== (6) GAME END CONDITION ======
    std::cout << "\n(6) Testing Game End Condition...\n";
    std::cout << "Player 1 conquers all territories.\n";
    C->setPlayer(p1);
    D->setPlayer(p1);
    p1->getTerritories()->push_back(C);
    p1->getTerritories()->push_back(D);

    engine->mainGameLoop();

    std::cout << "\n=== MAIN GAME LOOP TEST COMPLETE ===\n";

    delete engine;
}
