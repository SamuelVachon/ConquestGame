#include <iostream>
#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include <cstdlib>
#include <ctime>
#include "Card.h"

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

static void linkNeighbors(Territory* a, Territory* b) {
    // Map/Territory expects non-const lvalue string& for addEdgesNames
    string an = a->getName();
    string bn = b->getName();
    a->addEdgesNames(bn);
    b->addEdgesNames(an);
}

// --- REQUIRED BY PART 4 ---
// Demonstrates:
// (1) validate() runs before execute()
// (2) territory ownership transfers on successful Advance
// (3) award exactly one card per turn if at least one conquest occurs
// (4) Negotiate prevents attacks between the two players for the rest of the turn
// (5) Blockade doubles armies and transfers to Neutral
// (6) All orders can be issued by a player and executed
void testOrderExecution() {
    cout << "===== testOrderExecution() =====\n";

    // Seed RNG once for the battle simulation (Advance 60%/70%)
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // --- Build a tiny map: A - B - C   and a separate D (initially non-adjacent) ---
    string nA = "A", nB = "B", nC = "C", nD = "D";
    Territory* A = new Territory(nA, 0, 0, 0);
    Territory* B = new Territory(nB, 0, 0, 0);
    Territory* C = new Territory(nC, 0, 0, 0);
    Territory* D = new Territory(nD, 0, 0, 0);

    linkNeighbors(A, B);
    linkNeighbors(B, C);
    // D is isolated for bomb invalid-adjacency test (we'll link later)

    // --- Players ---
    Player* p1 = new Player("Alice");
    Player* p2 = new Player("Bob");

    // Ownership & initial armies
    A->setPlayer(p1);  A->setArmy(8);
    B->setPlayer(p2);  B->setArmy(4);
    C->setPlayer(p1);  C->setArmy(2);
    D->setPlayer(p2);  D->setArmy(7);

    // Mirror ownership lists so your validations (toDefend()) succeed
    p1->addTerritory(A);
    p1->addTerritory(C);
    p2->addTerritory(B);
    p2->addTerritory(D);

    // Reinforcement pools & hands
    p1->addReinforcements(10);
    p2->addReinforcements(5);
    p1->setHand(new Hand());
    p2->setHand(new Hand());

    // Track whether p1 already got a card this turn
    bool p1CardAwarded = false;

    // ---------------------------
    // (1) DEPLOY: valid + invalid
    // ---------------------------
    cout << "\n-- DEPLOY --\n";
    {
        DeployOrder depValid(p1, A, 5);   // own territory → valid, spends from pool
        depValid.execute();

        DeployOrder depInvalid(p1, B, 3); // enemy territory → invalid
        depInvalid.execute();
    }

    // ------------------------------------------
    // (4) NEGOTIATE prevents attacks this turn
    // ------------------------------------------
    cout << "\n-- NEGOTIATE --\n";
    {
        NegotiateOrder nego(p1, p2);
        nego.execute();

        // Attempt an attack while truce is active → should be invalid
        AdvanceOrder atkBlocked(p1, A, B, 3);
        atkBlocked.execute();

        // End of turn: reset truce + conquest flags for both players
        p1->resetTurnFlags();
        p2->resetTurnFlags();
    }

    // ------------------------------------------------------
    // (2) ADVANCE vs enemy + (3) award exactly ONE card if
    //     at least one conquest happened this turn
    // ------------------------------------------------------
    cout << "\n-- ADVANCE (battle & conquest + one-card rule) --\n";
    {
        // Make conquest likely: keep B small and move many from A
        A->setArmy(10);
        B->setArmy(2);

        AdvanceOrder atk(p1, A, B, 8);
        atk.execute();

        if (B->getPlayer() == p1) {
            cout << "[OK] Ownership of B transferred to " << p1->getName() << ".\n";
            if (p1->conqueredThisTurn() && !p1CardAwarded) {
                // Give exactly one card (type irrelevant per spec)
                p1->getHand()->addCard(new Card(CardType::Airlift));
                p1CardAwarded = true;
                cout << "Awarded 1 card to " << p1->getName()
                     << " for conquering at least one territory this turn.\n";
            }
        } else {
            cout << "[INFO] Attack failed; no territory conquered.\n";
        }
    }

    // -----------------------------------
    // AIRLIFT: own→own, non-adjacent OK
    // -----------------------------------
    cout << "\n-- AIRLIFT --\n";
    {
        // After conquest, B likely belongs to p1; move from C to B (or A if not)
        Territory* dest = (B->getPlayer() == p1) ? B : A;
        AirliftOrder air(p1, C, dest, 1);
        air.execute();
    }

    // -------------------------------------------------------------
    // BOMB: (invalid) own target, (invalid) no adjacency, (valid)
    // -------------------------------------------------------------
    cout << "\n-- BOMB --\n";
    {
        // Invalid: cannot bomb own territory
        BombOrder bombOwn(p1, A);
        bombOwn.execute();

        // Invalid: target not adjacent to any of p1's territories (D is isolated)
        BombOrder bombNoAdj(p1, D);
        bombNoAdj.execute();

        // Now make it adjacent and try again (valid)
        linkNeighbors(C, D);
        BombOrder bombOK(p1, D);
        bombOK.execute();
    }

    // ------------------------------------------------------
    // (5) BLOCKADE: double armies & transfer to Neutral
    // ------------------------------------------------------
    cout << "\n-- BLOCKADE --\n";
    {
        // Choose a p1 territory to blockade (A is safe)
        int before = A->getArmy();
        BlockadeOrder bl(p1, A);
        bl.execute();

        cout << "Blockade result: A armies " << before << " -> " << A->getArmy()
             << ", owner: " << (A->getPlayer() ? A->getPlayer()->getName() : string("(null)")) << "\n";
    }

    // ------------------------------------------------------
    // (6) Issue & execute via OrdersList (all orders in list)
    //     (Demonstrates they can be queued and executed)
    // ------------------------------------------------------
    cout << "\n-- ORDERS LIST PIPELINE --\n";
    {
        OrdersList ol;
        // Rebuild a few fresh orders to show list behavior
        DeployOrder* d = new DeployOrder(p2, D, 2);          // p2 deploy on D (after bomb may be halved)
        AdvanceOrder* a = new AdvanceOrder(p2, D, C, 2);     // p2 tries to attack C
        BlockadeOrder* b = new BlockadeOrder(p2, D);         // p2 blockades D
        ol.addOrder(d);
        ol.addOrder(a);
        ol.addOrder(b);

        cout << ol << endl;
        for (int i = 0; i < ol.size(); ++i) {
            cout << "\n>>> Executing queued order #" << i << endl;
            if (auto* o = ol.getOrder(i)) o->execute();
        }
    }

    // Cleanup
    delete A; delete B; delete C; delete D;
    delete p1; delete p2;

    cout << "===== end testOrderExecution() =====\n";
}


