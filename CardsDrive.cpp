#include "Card.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void testCards() {
    cout << "Creating deck with 20 cards" << endl;
    Deck deck(20);
    cout << "Deck size: " << deck.size() << endl;

    cout << "Drawing 5 cards into hand" << endl;
    Hand hand;

    for(int i = 0; i < 5; ++i) {
        Card* c = deck.draw();
        if(c) {
            hand.addCard(c);
        }
    }

    cout << "Hand Size: " << hand.size() << endl;
    cout << "Hand contents" << endl;

    hand.showCards();

    cout << "Playing all cards in hand..." << endl;
    for(int i = hand.size() - 1; i >= 0; --i) {
        //When this code was written , player wasn't fully implemented
        //As a result, use the playCard that doesn't use the player cla
        hand.playCard(i, &deck); // <-- REPLACE WHEN PLAYER IS IMPLEMENTED
    }

    cout << "Hand size after playing: " << hand.size() << endl;
    cout << "Deck size after returning cards: " << deck.size() << endl;
}

int main() {
    testCards();
    return 0;
}
