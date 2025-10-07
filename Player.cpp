#include "Player.h"
#include <iostream>
#include <algorithm>
#include "Card.h"     
#include "Orders.h"   

/* Default constructor: Creates a new player named "Player", empty container allocation
to territories, a new hand and a new OrdersList*/
Player::Player() 
: name_(new std::string("Player")),
  terrs_(new std::vector<Territory*>()),
  hand_(new Hand()),
  orders_(new OrdersList()) {}

//Parametrized constructor: Creates a player with a custom name but otherwise identical to the default constructor 
Player::Player(const std::string& name)
: name_(new std::string(name)),
  terrs_(new std::vector<Territory*>()),
  hand_(new Hand()),
  orders_(new OrdersList()) {}

/*Copy constructor: Initializes all pointer to nullptr, uses deepCopyForm to make a deep copy of 
another Player object (New dynamic memory allocation) */
Player::Player(const Player& other)
: name_(nullptr), terrs_(nullptr), hand_(nullptr), orders_(nullptr) {
    deepCopyFrom(other);
}

//Destructor : clears up all dynamically allocated data members -eg. Delete name
// Then sets each ointer to nullptr to avoid dangling references
Player::~Player() {
    delete name_;   name_ = nullptr;
    delete terrs_;  terrs_ = nullptr;
    delete hand_;   hand_ = nullptr;
    delete orders_; orders_ = nullptr;
}

/*Copy assignment operator: prevents self assignemt - Delete aby existing resources, then deep
copies the cintents another player */
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        delete name_;   name_ = nullptr;
        delete terrs_;  terrs_ = nullptr;
        delete hand_;   hand_ = nullptr;
        delete orders_; orders_ = nullptr;
        deepCopyFrom(other);
    }
    return *this;
}

//Getter: Returns the player’s name by dereferencing the std::string*
const std::string& Player::getName() const { return *name_; }

//toDefend(): Returns the list of territories the players currently owns
std::vector<Territory*> Player::toDefend() const {
    
    return *terrs_;
}

//toAttack(): Retruns the reverse of the owned territories list
std::vector<Territory*> Player::toAttack() const {
    
    auto v = *terrs_;
    std::reverse(v.begin(), v.end());
    return v;
}

//issueOrder():A player can create and queue an order. Creating a advanceOrder as a placeholder
void Player::issueOrder() {
    orders_->addOrder(new AdvanceOrder(this, nullptr, nullptr, 1));
}
//addTerritory(): Gives ownership of a territory to this player and stores the pointer.
//updates both sides of the relationship by calling t setPlayer this
void Player::addTerritory(Territory* t) {
    t->setPlayer(this);
    if (t) terrs_->push_back(t);
}

//setHand(): Assigns a new Hand to the player, deleting the old one if different.
//Makes sure that the player owns the passed-in Hand object
void Player::setHand(Hand* h) {
    if (hand_ == h) return;
    delete hand_;
    hand_ = h; // take ownership
}

//GetOrder(): Returns a pointer ti the player's current OrderList
OrdersList* Player::getOrders() const { return orders_; }


//Stream insertion: prints summary of the player's current state: name, nb of terrs,hand size, and nb of orders
std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player{name=" << *p.name_
       << ", territories=" << p.terrs_->size()
       << ", handSize=" << (p.hand_ ? p.hand_->size() : 0)
       << ", orders=" << (p.orders_ ? p.orders_->size() : 0)
       << "}";
    return os;
}
/* deepCopyForm(): Helper function used by copy constructor and assignment operator
-creates new memory for the player's name 
-Does shallow copy of terrs pointers - ownership stays eith the Map
-Does deep copies of Hand and Orderlist so each player has its own
*/
void Player::deepCopyFrom(const Player& other) {
    name_  = new std::string(*other.name_);
    // We don't own Territory*, so shallow-copy the pointers
    terrs_ = new std::vector<Territory*>(other.terrs_->begin(), other.terrs_->end());
    // We own Hand and OrdersList → deep-copy them
    hand_  = new Hand(*other.hand_);
    orders_= new OrdersList(*other.orders_);
}
