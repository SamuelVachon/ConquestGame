#include "Card.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
/**
    Card Implementation
*/
Card::Card(CardType type) : type(type) {}

Card::~Card() {}

Card::Card(const Card& other) : type(other.type) {}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        this->type = other.type;
    }
    return *this;
}

void Card::setType(CardType type){
    if(type != this->type){
        this->type = type;
    }
}


CardType Card::getType(){
    return type;
}

/**
    In case that there is ever a need to print the value
    or use the value as a string
*/
string Card::getTypeAsString() const {
    switch (type) {
        case CardType::Bomb: return "Bomb";
        case CardType::Reinforcement: return "Reinforcement";
        case CardType::Blockade: return "Blockade";
        case CardType::Airlift: return "Airlift";
        case CardType::Diplomacy: return "Diplomacy";
        default: return "Unknown";
    }
}

void Card::play(Player* player, Deck* deck){
    if(!deck){
        throw std::invalid_argument("Cannot play card, deck cannot be null!");
    }
    if(!player){
        throw std::invalid_argument("Cannot play card, player cannot be null!");
    }
    cout << "Playing " << getTypeAsString() << endl;
    deck->returnCard(this);
}

void Card::play(Hand* hand, Deck* deck){
    if(!deck){
       throw std::invalid_argument("Cannot play card, deck cannot be null!");
    }
    if(!hand){
        throw std::invalid_argument("Cannot play card, hand cannot be null!");
    }
    cout << "Playing " << getTypeAsString() << endl;
    hand->removeCard(this);
    deck->returnCard(this);
}

ostream& operator<<(ostream& out, const Card& c){
    out << c.getTypeAsString();
    return out;
}

/**
    Deck Class
*/

Deck::Deck(int size): cards{} {
    for(int i=0;i<size;i++){
        //modulus to alternate for each type of card
        cards.push_back(new Card(static_cast<CardType>(i%5)));
    }
}

Deck::~Deck(){
    for(Card* c : cards){
        delete c;
    }
    cards.clear();
}

int Deck::size() const{
    return cards.size();
}

Card* Deck::draw(){
    if(cards.empty()){
        throw std::out_of_range("Cannot draw a card, the deck is empty");
    }

    int idx = rand() % size();

    Card* card = cards[idx];

    cards.erase(cards.begin() + idx);
    return card;
}

void Deck::returnCard(Card* card){
    cards.push_back(card);
}

/**
    Hand Implementation
*/

Hand::Hand(): handCards{} {}

Hand::~Hand(){
    for(Card* c : handCards){
        delete c;
    }
    handCards.clear();
}

Hand::Hand(const Hand& other) {
    for (Card* c : other.handCards) {
        handCards.push_back(new Card(*c));
    }
}

Hand& Hand::operator=(const Hand& other){
    if(this != &other){
        for (Card* c : handCards) delete c;
        handCards.clear();
        for(Card* c : other.handCards){
            handCards.push_back(new Card(*c));
        }
    }
    return *this;
}

int Hand::size() const{
    return handCards.size();
}

void Hand::showCards(){
    for(Card* c : handCards){
        cout << "Hand shows his card: " << c->getTypeAsString() << endl;
    }
}

void Hand::addCard(Card* card){
    if(!card){
        throw std::invalid_argument("Card cannot be null!");
    }
    handCards.push_back(card);
}

void Hand::removeCard(Card* card){
    //Look for the first card that matches pointer
    auto it = std::find(handCards.begin(), handCards.end(), card);
    if (it == handCards.end()){
        throw std::invalid_argument("The card does not exist!");
    }
    handCards.erase(it);
}

//Modify when player class is implemented (add order based of card to player list)
 void Hand::playCard(int index, Player* player, Deck* deck){
    if(index > size() || index < -1){
        cout << "Play card must play a card that exists in the hand!" << endl;
        exit(1);
    }
    Card* card = handCards[index];

    card->play(player, deck);
 }

 //Only to be used at the start of coding WHEN THE PLAYER CLASS ISN'T yet available
 void Hand::playCard(int index, Deck* deck){
    if(index >= size() || index < 0 ){
        throw std::invalid_argument("Play card must play a card that exists in the hand!");
    }
    Card* card = handCards[index];

    card->play(this, deck);
 }


