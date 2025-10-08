#pragma once      
//Making sure header file is only included once per complilation unit 

#include <iosfwd> 
#include <vector>
#include <string>

class Territory;     // from Map (forward-declared only)
class Hand;          // from Card.h (teammate)
class OrdersList;    // defined in our Orders.{h,cpp}
class Order;

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
    void setHand(Hand* h);                   // takes ownership of Hand*
    OrdersList* getOrders() const;           // non-owning access

    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const Player& p); // prints player summary(name territory, hand size,order count)

private:
    // Per assignment: user-defined members as pointer types
    std::string* name_;   
    std::vector<Territory*>* terrs_;
    Hand*        hand_;
    OrdersList*  orders_;

// Internal helper
    void deepCopyFrom(const Player& other); // deep copy used in copy constructor
};
