#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED
#include <stdlib.h>
#include <string>
    using std::string;
#include <vector>
    using std::vector;
class Player;
class Deck;
class Hand;

enum class CardType{
    Bomb,
    Reinforcement,
    Blockade,
    Airlift,
    Diplomacy
};

class Card{
    private:
        CardType type;
    public:
        Card(CardType type);
        ~Card();


        Card(const Card& other);
        Card& operator=(const Card& other);
        CardType getType();
        void setType(CardType type);

        string getTypeAsString() const;

        void play(Player* player, Deck* deck);

        void play(Hand* hand, Deck* deck);
        friend std::ostream& operator<<(std::ostream& out, const Card& c);
};

class Deck {
    private:
        vector<Card*> cards;
    public:
        Deck(int size);
        ~Deck();

        Deck(const Deck& other);
        Deck& operator=(const Deck& other);

        Card* draw();
        void returnCard(Card* card);
        int size() const;

        friend std::ostream& operator<<(std::ostream& out, const Deck& d);
};


class Hand{
    private:
        vector<Card*> handCards;
    public:
        Hand();
        ~Hand();

        Hand(const Hand& other);
        Hand& operator=(const Hand& other);

        void addCard(Card* card);
        void removeCard(Card* card);

        void playCard(int index, Player* player, Deck* deck);
        /**
            This implementation of playCard was only created to be used when the player class
            wasn't implemented, when said class is finished and holds it's own hand class,

            @param index Index of the card to play
            @param deck Deck the card will return to
        */
        void playCard(int index, Deck* deck);
        void showCards();

        int size() const;

        friend std::ostream& operator<<(std::ostream& out, const Hand& h);
};

#endif // CARD_H_INCLUDED
