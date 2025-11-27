#include "Player.h"
#include <iostream>
#include <algorithm>
#include "Card.h"           // Hand, Card
#include "Orders.h"         // OrdersList, DeployOrder, AdvanceOrder, etc.
#include "Map.h"            // Territory
#include "PlayerStrategies.h"

// ======================
//   Player implementation
// ======================

/* Default constructor: Creates a new player named "Player", empty container allocation
   to territories, a new hand and a new OrdersList */
Player::Player()
    : name_(new std::string("Player")),
      terrs_(new std::vector<Territory*>()),
      hand_(new Hand()),
      orders_(new OrdersList()),
      reinforcemtnts_(50),
      strategy_(nullptr)
{
    // You can default to Human strategy if you want:
    // setStrategy(new HumanPlayerStrategy(this));
}

// Parametrized constructor: Creates a player with a custom name but otherwise identical
Player::Player(const std::string& name)
    : name_(new std::string(name)),
      terrs_(new std::vector<Territory*>()),
      hand_(new Hand()),
      orders_(new OrdersList()),
      reinforcemtnts_(50),
      strategy_(nullptr)
{
    // setStrategy(new HumanPlayerStrategy(this));
}

/* Copy constructor: initializes pointers to nullptr, then deep-copy from other */
Player::Player(const Player& other)
    : name_(nullptr),
      terrs_(nullptr),
      hand_(nullptr),
      orders_(nullptr),
      reinforcemtnts_(0),
      strategy_(nullptr)
{
    deepCopyFrom(other);
}

// Destructor: clears all dynamically allocated members
Player::~Player() {
    delete name_;    name_ = nullptr;
    delete terrs_;   terrs_ = nullptr;
    delete hand_;    hand_ = nullptr;
    delete orders_;  orders_ = nullptr;
    delete strategy_; strategy_ = nullptr;
}

/* Copy assignment operator */
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        delete name_;    name_ = nullptr;
        delete terrs_;   terrs_ = nullptr;
        delete hand_;    hand_ = nullptr;
        delete orders_;  orders_ = nullptr;
        delete strategy_; strategy_ = nullptr;

        deepCopyFrom(other);
    }
    return *this;
}

// Getter: returns the player’s name
const std::string& Player::getName() const {
    return *name_;
}

// ============= Strategy-aware behavior =============

// toDefend(): normally calls strategy; fallback = all owned territories
std::vector<Territory*> Player::toDefend() const {
    if (strategy_) {
        return strategy_->toDefend();
    }
    // fallback: all territories
    return *terrs_;
}

// toAttack(): normally calls strategy; fallback = reverse owned territories
std::vector<Territory*> Player::toAttack() const {
    if (strategy_) {
        return strategy_->toAttack();
    }
    // fallback: reverse of owned territories
    auto v = *terrs_;
    std::reverse(v.begin(), v.end());
    return v;
}

// issueOrder(): if a strategy exists, delegate to it; otherwise use placeholder
void Player::issueOrder() {
    if (strategy_) {
        // Strategies have issueOrder(Deck*), but we don't have a Deck here,
        // so we pass nullptr. The strategy methods are written to ignore it.
        strategy_->issueOrder(nullptr);
    } else {
        // Original placeholder behavior from A1/A2:
        orders_->addOrder(new AdvanceOrder(this, nullptr, nullptr, 1));
    }
}

// ============= Helpers / basic operations =============

// Gives ownership of a territory to this player and stores the pointer.
void Player::addTerritory(Territory* t) {
    if (!t) return;
    t->setPlayer(this);
    terrs_->push_back(t);
}

// setHand(): assigns a new Hand to the player, deleting the old one if different.
void Player::setHand(Hand* h) {
    if (hand_ == h) return;
    delete hand_;
    hand_ = h;  // take ownership
}

// getOrders(): returns pointer to the player’s OrdersList
OrdersList* Player::getOrders() const {
    return orders_;
}

// getTerritories(): returns pointer to vector of owned territories
std::vector<Territory*>* Player::getTerritories() {
    return terrs_;
}

void Player::addOrder(Order* order){
    if(order != nullptr){
        orders_->addOrder(order);
    }
}

// ============= Strategy pattern API =============

void Player::setStrategy(PlayerStrategy* s) {
    if (strategy_ == s) return;
    delete strategy_;
    strategy_ = s;
    // strategy_ is already constructed with this Player* in its ctor
}

PlayerStrategy* Player::getStrategy() const {
    return strategy_;
}

// ============= Output operator =============

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player{name=" << *p.name_
       << ", territories=" << p.terrs_->size()
       << ", handSize=" << (p.hand_ ? p.hand_->size() : 0)
       << ", orders=" << (p.orders_ ? p.orders_->size() : 0)
       << "}";
    return os;
}

// ============= Deep copy helper =============

void Player::deepCopyFrom(const Player& other) {
    name_  = new std::string(*other.name_);
    // We don't own Territory*, so shallow-copy the pointers
    terrs_ = new std::vector<Territory*>(other.terrs_->begin(), other.terrs_->end());
    // We own Hand and OrdersList → deep-copy them
    hand_  = new Hand(*other.hand_);
    orders_= new OrdersList(*other.orders_);
    reinforcemtnts_ = other.reinforcemtnts_;

    // A2 Part 4 fields
    reinforce_          = other.reinforce_;
    conqueredThisTurn_  = other.conqueredThisTurn_;
    negotiated_         = other.negotiated_;

    // Strategy pointer is NOT deep-copied here; new copy starts without strategy
    strategy_ = nullptr;
}

// ============= A2 Part 4: Reinforcements, flags, negotiation =============

Hand* Player::getHand() const {
    return hand_;
}

// Reinforcement pool
void Player::addReinforcements(int n) {
    reinforce_ += n;
}

bool Player::spendReinforcements(int n) {
    if (n <= reinforce_) {
        reinforce_ -= n;
        return true;
    }
    return false;
}

int Player::reinforcementPool() const {
    return reinforce_;
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
    if (p && p != this) {
        negotiated_.push_back(p);
    }
}

bool Player::hasTruceWith(Player* p) const {
    for (auto* q : negotiated_) {
        if (q == p) return true;
    }
    return false;
}

void Player::issueOrder(Deck* deck) {
    if (strategy_) {
        strategy_->issueOrder(deck);
    } else {
        // fallback for A1/A2 compatibility
        orders_->addOrder(new AdvanceOrder(this, nullptr, nullptr, 1));
    }
}
