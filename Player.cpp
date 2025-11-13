#include "Player.h"
#include <iostream>
#include <algorithm>
#include "Card.h"
#include "Orders.h"
#include "Map.h"

/* Default constructor: Creates a new player named "Player", empty container allocation
to territories, a new hand and a new OrdersList*/
Player::Player()
: name_(new std::string("Player")),
  terrs_(new std::vector<Territory*>()),
  hand_(new Hand()),
  orders_(new OrdersList()),
  reinforcements_(50),
  deployableUnits(0),
  doneIssuing_(false) {}

//Parametrized constructor: Creates a player with a custom name but otherwise identical to the default constructor
Player::Player(const std::string& name)
: name_(new std::string(name)),
  terrs_(new std::vector<Territory*>()),
  hand_(new Hand()),
  orders_(new OrdersList()),
  reinforcements_(50),
  deployableUnits(0),
  doneIssuing_(false) {}

/*Copy constructor: Initializes all pointer to nullptr, uses deepCopyForm to make a deep copy of
another Player object (New dynamic memory allocation) */
Player::Player(const Player& other)
: name_(nullptr), terrs_(nullptr), hand_(nullptr), orders_(nullptr), reinforcements_(0),deployableUnits(0),doneIssuing_(false) {
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

//Modified for Part 3
//toAttack(): Retruns the reverse of the owned territories list
std::vector<Territory*> Player::toAttack() const {
    std::vector<Territory*> targets;
    std::vector<Territory*>::iterator it;
    for(it = terrs_->begin(); it !=terrs_->end(); it++){
        std::vector<Territory*> adjacentTerritories = (*it)->getAdjacentTerritories();
        for(Territory* adjTerr: adjacentTerritories){
                //Checks if adjacent isn't owned by the player, and the territory isn't in the target list
               if(this != adjTerr->getPlayer() && std::find(targets.begin(), targets.end(), (*it)) == targets.end() ){
                    targets.push_back(adjTerr);
                }
        }
    }
    return targets;
}

//issueOrder():A player can create and queue an order. Creating a advanceOrder as a placeholder
void Player::issueOrder() {
    orders_->addOrder(new AdvanceOrder(this, nullptr, nullptr, 1));
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
std::vector<Territory*>* Player::getTerritories(){ return terrs_; }

//addTerritory(): Gives ownership of a territory to this player and stores the pointer.
//updates both sides of the relationship by calling t setPlayer this
void Player::addTerritory(Territory* t) {
    t->setPlayer(this);
    if (t) terrs_->push_back(t);
}


// ====== A2 Part 3 Modifications ======

void Player::issueOrder(Deck* deck){
    if (reinforcements_ > 0) {
        Territory* target = toDefend().front();
        int deployCount = min(3, reinforcements_);
        orders_->addOrder(new DeployOrder(this, target, deployCount));
        reinforcements_ -= deployCount;
        deployableUnits += deployCount;
        std::cout << *name_ << " issues Deploy(" << deployCount
             << " to " << target->getName() << ")\n";
        if (reinforcements_ == 0)
            doneIssuing_ = false; // still can issue other orders
        return;
    }

    //Decide wether the user can attack and/or defend
    bool doAttack = !toAttack().empty();
    bool doDefend = toDefend().size() > 1; // need at least 2 territories to move

    // after deploys are done
    if(doDefend){
        Territory* src = toDefend().back();
        Territory* tgt = toDefend().front();
        orders_->addOrder(new AdvanceOrder(this, src, tgt, 2));

        std::cout << *name_ << " issues Defensive Advance("
                  << src->getName() << " -> " << tgt->getName() << ")\n";
    }

    if (doAttack) {
        Territory* src = toDefend().front();
        Territory* tgt = toAttack().front();
        orders_->addOrder(new AdvanceOrder(this, src, tgt, 2));
        std::cout << *name_<< " issues Advance(" << src->getName()
             << " -> " << tgt->getName() << ")\n";
    }

    // optionally play a card
    if (hand_->size()>0) {

        hand_->playCard(hand_->size(), this, deck);
    }

    doneIssuing_ = true;
}

bool Player::isDoneIssuing() const { return doneIssuing_; }
bool Player::hasOrders() const { return !(orders_->size() < 1); }
Order* Player::nextOrder() {
    //this check should in theory not be run since hasOrders can be called before calling this
    //still is a good idea just to be sure
    if (!orders_ || orders_->size() == 0)
        return nullptr;

    int top = orders_->size() - 1;
    return orders_->getOrder(top);
}

void Player::addOrder(Order* order){
    if(order != nullptr){
        orders_->addOrder(order);
    }
}

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
    reinforcements_ = other.reinforcements_;
    doneIssuing_ = other.doneIssuing_;
}

// ====== A2 Part 4 Additions ======

Hand* Player::getHand() const {
    return hand_;
}

// Reinforcement pool
void Player::addReinforcements(int n) {
    reinforcements_ += n;
}

//Helper mostly used for testing
void Player::setReinforcements(int n){
    reinforcements_ = n;
}

bool Player::spendDeployableUnits(int n) {
    if (n <= deployableUnits) {
        deployableUnits -= n;
        return true;
    }
    return false;
}

int Player::getDeployableUnits(){
    return deployableUnits;
}
void Player::addDeployableUnits(int n){
    deployableUnits += n;
}

int Player::reinforcementPool() const {
    return reinforcements_;
}

// Turn/conquest flags
void Player::markConquered() {
    conqueredThisTurn_ = true;
}

bool Player::conqueredThisTurn() const {
    return conqueredThisTurn_;
}

void Player::resetTurnFlags() {
    conqueredThisTurn_ = false;
    negotiated_.clear();
}

// Negotiation (truce)
void Player::addTruceWith(Player* p) {
    if (p && p != this)
        negotiated_.push_back(p);
}

bool Player::hasTruceWith(Player* p) const {
    for (auto* q : negotiated_) {
        if (q == p) return true;
    }
    return false;
}
