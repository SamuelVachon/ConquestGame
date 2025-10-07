#pragma once
#include <iosfwd>
#include <vector>
#include <string>

class Territory;     // from Map (forward-declared only)
class Hand;          // from Card.h (teammate)
class OrdersList;    // defined in our Orders.{h,cpp}
class Order;

class Player {
public:
    Player();                                // default
    explicit Player(const std::string& name);
    Player(const Player& other);             // copy ctor
    ~Player();                               // dtor
    Player& operator=(const Player& other);  // copy assign

    const std::string& getName() const;

    std::vector<Territory*> toDefend() const;
    std::vector<Territory*> toAttack() const;
    void issueOrder();                       // creates an Order and adds to list

    // Driver helpers
    void addTerritory(Territory* t);         // Player does NOT own Territory*
    void setHand(Hand* h);                   // takes ownership of Hand*
    OrdersList* getOrders() const;           // non-owning access

    // Pretty print
    friend std::ostream& operator<<(std::ostream& os, const Player& p);

private:
    // Per assignment: user-defined members as pointer types
    std::string* name_;
    std::vector<Territory*>* terrs_;
    Hand*        hand_;
    OrdersList*  orders_;

    void deepCopyFrom(const Player& other);
};
