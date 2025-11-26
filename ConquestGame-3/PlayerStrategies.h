#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include <vector>
#include <string>

class Player;
class Territory;
class Deck;

class PlayerStrategy {
public:
    explicit PlayerStrategy(Player* player);
    virtual ~PlayerStrategy() = default;

    virtual std::vector<Territory*> toDefend() = 0;
    virtual std::vector<Territory*> toAttack() = 0;
    virtual void issueOrder(Deck* deck) = 0;

    virtual PlayerStrategy* clone(Player* newOwner) const = 0;
    virtual std::string getName() const = 0;

protected:
    Player* player_;   // non-owning pointer to the player using this strategy
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* player);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;

    PlayerStrategy* clone(Player* newOwner) const override;
    std::string getName() const override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* player);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;

    PlayerStrategy* clone(Player* newOwner) const override;
    std::string getName() const override;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* player);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;

    PlayerStrategy* clone(Player* newOwner) const override;
    std::string getName() const override;
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* player);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;

    PlayerStrategy* clone(Player* newOwner) const override;
    std::string getName() const override;
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* player);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;

    PlayerStrategy* clone(Player* newOwner) const override;
    std::string getName() const override;
};

// Required driver
void testPlayerStrategies();

#endif // PLAYER_STRATEGIES_H

