#include <iostream>
#include "Orders.h"
#include "Map.h"
#include "Player.h"

using namespace std;

// FUNCTION: testOrdersLists()

void testOrdersLists() {

    cout << "          TESTING ORDERS LIST MODULE         \n";

    // --- 1. Create Players ---
    Player* alice = new Player("Alice");
    Player* bob   = new Player("Bob");
    Player* carol = new Player("Carol");

    // --- 2. Create Territories (for testing only) ---
    string t1Name = "Quebec";
    string t2Name = "Ontario";
    string t3Name = "Manitoba";
    string t4Name = "Alberta";

    Territory* t1 = new Territory(t1Name, 0, 0, 1);
    Territory* t2 = new Territory(t2Name, 0, 0, 1);
    Territory* t3 = new Territory(t3Name, 0, 0, 1);
    Territory* t4 = new Territory(t4Name, 0, 0, 1);

    // Assign ownership
    t1->setPlayer(alice);
    t2->setPlayer(alice);
    t3->setPlayer(bob);
    t4->setPlayer(carol);

    // Set starting armies
    t1->setArmy(5);
    t2->setArmy(3);
    t3->setArmy(8);
    t4->setArmy(2);

    // Add territories to players (so validate() checks succeed)
    alice->addTerritory(t1);
    alice->addTerritory(t2);
    bob->addTerritory(t3);
    carol->addTerritory(t4);

    // --- 3. Create Various Orders ---
    DeployOrder* deploy    = new DeployOrder(alice, t1, 4);
    AdvanceOrder* advance  = new AdvanceOrder(alice, t1, t2, 2);
    BombOrder* bomb        = new BombOrder(alice, t3);
    BlockadeOrder* blockade= new BlockadeOrder(alice, t2);
    AirliftOrder* airlift  = new AirliftOrder(alice, t1, t2, 3);
    NegotiateOrder* nego   = new NegotiateOrder(alice, bob);

    // --- 4. Create an OrdersList ---
    OrdersList ordersList;

    // --- 5. Add orders ---
    cout << "\n--- Adding Orders ---\n";
    ordersList.addOrder(deploy);
    ordersList.addOrder(advance);
    ordersList.addOrder(bomb);
    ordersList.addOrder(blockade);
    ordersList.addOrder(airlift);
    ordersList.addOrder(nego);

    cout << ordersList << endl;

    // --- 6. Move orders ---
    cout << "\n--- Moving Orders (Advance from index 1 to 4) ---\n";
    ordersList.moveOrder(1, 4);
    cout << ordersList << endl;

    // --- 7. Execute all orders ---
    cout << "\n--- Executing Orders ---\n";
    for (int i = 0; i < ordersList.size(); ++i) {
        Order* o = ordersList.getOrder(i);
        if (o) {
            cout << "\n>>> Executing order #" << i << endl;
            o->execute();
            cout << *o << endl;
        }
    }

    // --- 8. Remove a few orders ---
    cout << "\n--- Removing order at index 2 ---\n";
    ordersList.removeOrder(2);
    cout << ordersList << endl;

    // --- 9. Cleanup ---
    delete alice;
    delete bob;
    delete carol;
    delete t1;
    delete t2;
    delete t3;
    delete t4;

     cout << "       END OF ORDERS LIST DEMONSTRATION     \n";
};
