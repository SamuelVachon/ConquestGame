#pragma once
// Ensures this header is only included once

#include <iosfwd>
#include <vector>
#include <string>

class Territory;     // from Map
class Hand;          // from Card.h
class OrdersList;    // from Orders.h
class Order;         
class PlayerStrategy; // <-- Strategy pattern
class Deck;  

void testPlayers();

/*
   Class Player:
   - Owns territories
   - Holds a Hand of cards
   - Maintains an OrdersList
   - Issues orders (possibly using a strategy)
*/
class Player {
public:
    // Constructors, destructor, assignment
    Player();
    explicit Player(const std::string& name);
    Player(const Player& other);
    ~Player();
    Player& operator=(const Player& other);

    // Basic getter
    const std::string& getName() const;

    // Strategy-aware behavior
    std::vector<Territory*> toDefend() const;
    std::vector<Territory*> toAttack() const;
    void issueOrder();   // uses current strategy if available

    // Territory management
    void addTerritory(Territory* t);
    std::vector<Territory*>* getTerritories();   // <-- required for strategies

    // Hand & Orders
    void setHand(Hand* h);          
    Hand* getHand() const;          
    OrdersList* getOrders() const;

    // Reinforcements
    void addReinforcements(int n);
    bool spendReinforcements(int n);
    int reinforcementPool() const;

    // Conquest / turn flags
    void markConquered();
    bool conqueredThisTurn() const;
    void resetTurnFlags();

    // Negotiation / truce
    void addTruceWith(Player* p);
    bool hasTruceWith(Player* p) const;

    // Strategy control
    void setStrategy(PlayerStrategy* s);    
    PlayerStrategy* getStrategy() const;

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
     void issueOrder(Deck* deck);

private:
    // Required pointer-based members
    std::string* name_;
    std::vector<Territory*>* terrs_;
    Hand*        hand_;
    OrdersList*  orders_;
    int reinforcemtnts_;     // from your file (kept as-is)

    // A2 fields
    int reinforce_ = 0;
    bool conqueredThisTurn_ = false;
    std::vector<Player*> negotiated_;

    // Strategy pattern
    PlayerStrategy* strategy_;   // <-- added

    // Internal deep copy helper
    void deepCopyFrom(const Player& other);
   
};


