#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include <vector>
#include <string>

class Player;
class Territory;

class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;

    virtual std::vector<Territory*> toDefend(Player* player) = 0;
    virtual std::vector<Territory*> toAttack(Player* player) = 0;
    virtual void issueOrder(Player* player) = 0;

    virtual PlayerStrategy* clone() const = 0;
    virtual std::string getName() const = 0;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    void issueOrder(Player* player) override;

    PlayerStrategy* clone() const override;
    std::string getName() const override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    void issueOrder(Player* player) override;

    PlayerStrategy* clone() const override;
    std::string getName() const override;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    void issueOrder(Player* player) override;

    PlayerStrategy* clone() const override;
    std::string getName() const override;
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    void issueOrder(Player* player) override;

    PlayerStrategy* clone() const override;
    std::string getName() const override;
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    std::vector<Territory*> toDefend(Player* player) override;
    std::vector<Territory*> toAttack(Player* player) override;
    void issueOrder(Player* player) override;

    PlayerStrategy* clone() const override;
    std::string getName() const override;
};

// Required driver
void testPlayerStrategies();

#endif // PLAYER_STRATEGIES_H
