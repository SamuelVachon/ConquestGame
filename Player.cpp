#include "Player.h"
#include <iostream>
#include <algorithm>
#include "Card.h"     // teammate's Hand/Card
#include "Orders.h"   // our OrdersList/Order declarations

Player::Player()
: name_(new std::string("Player")),
  terrs_(new std::vector<Territory*>()),
  hand_(new Hand()),
  orders_(new OrdersList()) {}

Player::Player(const std::string& name)
: name_(new std::string(name)),
  terrs_(new std::vector<Territory*>()),
  hand_(new Hand()),
  orders_(new OrdersList()) {}

Player::Player(const Player& other)
: name_(nullptr), terrs_(nullptr), hand_(nullptr), orders_(nullptr) {
    deepCopyFrom(other);
}

Player::~Player() {
    delete name_;   name_ = nullptr;
    delete terrs_;  terrs_ = nullptr;
    delete hand_;   hand_ = nullptr;
    delete orders_; orders_ = nullptr;
}

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

const std::string& Player::getName() const { return *name_; }

std::vector<Territory*> Player::toDefend() const {
    // demo logic: return owned territories as-is
    return *terrs_;
}

std::vector<Territory*> Player::toAttack() const {
    // demo logic: reversed list as placeholder
    auto v = *terrs_;
    std::reverse(v.begin(), v.end());
    return v;
}

void Player::issueOrder() {
    orders_->addOrder(new AdvanceOrder(this, nullptr, nullptr, 1));
}

void Player::addTerritory(Territory* t) {
    if (t) terrs_->push_back(t);
}

void Player::setHand(Hand* h) {
    if (hand_ == h) return;
    delete hand_;
    hand_ = h; // take ownership
}

OrdersList* Player::getOrders() const { return orders_; }

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player{name=" << *p.name_
       << ", territories=" << p.terrs_->size()
       << ", handSize=" << (p.hand_ ? p.hand_->size() : 0)
       << ", orders=" << (p.orders_ ? p.orders_->size() : 0)
       << "}";
    return os;
}

void Player::deepCopyFrom(const Player& other) {
    name_  = new std::string(*other.name_);
    // We don't own Territory*, so shallow-copy the pointers
    terrs_ = new std::vector<Territory*>(other.terrs_->begin(), other.terrs_->end());
    // We own Hand and OrdersList → deep-copy them
    hand_  = new Hand(*other.hand_);
    orders_= new OrdersList(*other.orders_);
}