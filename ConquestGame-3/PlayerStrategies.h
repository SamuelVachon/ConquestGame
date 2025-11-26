#pragma once

#include <vector>
#include <string>

class Player;
class Territory;
class Deck;

// ---------- Base Strategy ----------

class PlayerStrategy {
public:
    explicit PlayerStrategy(Player* p);
    PlayerStrategy(const PlayerStrategy& other);
    virtual ~PlayerStrategy();
    PlayerStrategy& operator=(const PlayerStrategy& other);

    Player* getPlayer() const;
    void setPlayer(Player* p);

    // pure virtual behaviour
    virtual std::vector<Territory*> toDefend() = 0;
    virtual std::vector<Territory*> toAttack() = 0;
    virtual void issueOrder(Deck* deck) = 0;

protected:
    Player* player_;
};

// ---------- Concrete Strategies ----------

// Human = same behaviour we currently have in Player::issueOrder/toDefend/toAttack
class HumanPlayerStrategy : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* p);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;
    void reinforcePhase();
    void attackPhase();
    void cardPhase(Deck* deck);
    char choice(const std::string& type);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* p);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* p);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* p);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;
};

// Cheater: conquers all adjacent enemy territories automatically
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* p);

    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;
    void issueOrder(Deck* deck) override;
};


void testPlayerStrategies();
