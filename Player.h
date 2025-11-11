#pragma once
//Making sure header file is only included once per complilation unit

#include <iosfwd>
#include <vector>
#include <string>

class Territory;     // from Map (forward-declared only)
class Hand;          // from Card.h (teammate)
class OrdersList;    // defined in our Orders.{h,cpp}
class Order;         // defined in our Orders.{h,cpp}
class Deck;          // from Card.h

void testPlayers();

/*Class player represents a player in the game
A player owns a set of territories, holds a hand of cards,
maintains a list of Orders, and can issue orders*/
class Player {
public:
    Player();                                // default constuctor ( create generic player
    explicit Player(const std::string& name);
    Player(const Player& other);             // copy constructor(deep copies another player)
    ~Player();                               // destructor (release dynamic memory)
    Player& operator=(const Player& other);  // copy assignment operator

    const std::string& getName() const; // return player name

    // returns list of territories owned and to attack
    std::vector<Territory*> toDefend() const;
    std::vector<Territory*> toAttack() const;
    void issueOrder();                       // creates an Order and adds to list

    // Driver helpers
    void addTerritory(Territory* t);         // Player does NOT own Territory*
    std::vector<Territory*>* getTerritories();
    void addReinforcements(int number);

    //Related to main game loop
    void issueOrder(Deck* deck);
    bool isDoneIssuing() const;
    bool hasOrders() const;
    void addOrder(Order* order);
    Order* nextOrder();

    void setHand(Hand* h);                   // takes ownership of Hand*
    OrdersList* getOrders() const;           // non-owning access

    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const Player& p); // prints player summary(name territory, hand size,order count)

    // ====== A2 Part 4 Additions ======

    // Reinforcement pool
    void addReinforcements(int n);
    bool spendReinforcements(int n);
    int reinforcementPool() const;

    // Turn/conquest flags
    void markConquered();
    bool conqueredThisTurn() const;
    void resetTurnFlags();

    // Negotiation (truce)
    void addTruceWith(Player* p);
    bool hasTruceWith(Player* p) const;

    // Hand getter
    Hand* getHand() const;

private:
    // Per assignment: user-defined members as pointer types
    std::string* name_;
    std::vector<Territory*>* terrs_;
    Hand*        hand_;
    OrdersList*  orders_;
    int reinforcemtnts_;
    bool doneIssuing_;
// Internal helper
    void deepCopyFrom(const Player& other); // deep copy used in copy constructor
    // ====== A2 Part 4 Fields ======
    int reinforce_ = 0;
    bool conqueredThisTurn_ = false;
    std::vector<Player*> negotiated_;

};

