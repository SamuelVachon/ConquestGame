#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Card.h"
#include <algorithm>
#include <iostream>

// ===== PlayerStrategy base =====

PlayerStrategy::PlayerStrategy(Player* p) : player_(p) {}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& other)
    : player_(other.player_) {}

PlayerStrategy::~PlayerStrategy() = default;

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& other) {
    if (this != &other) {
        player_ = other.player_;
    }
    return *this;
}

Player* PlayerStrategy::getPlayer() const { return player_; }
void PlayerStrategy::setPlayer(Player* p) { player_ = p; }

// small helpers
static Territory* strongestTerritory(Player* p) {
    auto terrs = *p->getTerritories();
    if (terrs.empty()) return nullptr;
    return *std::max_element(
        terrs.begin(), terrs.end(),
        [](Territory* a, Territory* b){ return a->getArmy() < b->getArmy(); });
}

static Territory* weakestTerritory(Player* p) {
    auto terrs = *p->getTerritories();
    if (terrs.empty()) return nullptr;
    return *std::min_element(
        terrs.begin(), terrs.end(),
        [](Territory* a, Territory* b){ return a->getArmy() < b->getArmy(); });
}

// ===== HumanPlayerStrategy =====

HumanPlayerStrategy::HumanPlayerStrategy(Player* p)
    : PlayerStrategy(p) {}

std::vector<Territory*> HumanPlayerStrategy::toDefend() {
    // same as your old Player::toDefend()  (all owned territories) :contentReference[oaicite:0]{index=0}
    return *player_->getTerritories();
}

std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    // same as your old Player::toAttack() logic (adjacent enemy territories) :contentReference[oaicite:1]{index=1}
    std::vector<Territory*> targets;
    for (Territory* t : *player_->getTerritories()) {
        for (Territory* adj : t->getAdjacentTerritories()) {
            if (adj->getPlayer() != player_ &&
                std::find(targets.begin(), targets.end(), adj) == targets.end()) {
                targets.push_back(adj);
            }
        }
    }
    return targets;
}

void HumanPlayerStrategy::issueOrder(Deck* deck) {
    // This is basically your old Player::issueOrder(Deck*) behaviour,
    // just moved here. :contentReference[oaicite:2]{index=2}

    if (player_->reinforcementPool() > 0) {
    auto defendList = toDefend();
    Territory* target = defendList.empty() ? nullptr : defendList.front();
    if (target) {
        int deployCount = std::min(3, player_->reinforcementPool());

        // Spend from the pool using your existing API
        if (player_->spendReinforcements(deployCount)) {
            player_->getOrders()->addOrder(
                new DeployOrder(player_, target, deployCount)
            );

            std::cout << player_->getName() << " [Human] Deploy "
                      << deployCount << " to " << target->getName() << "\n";
        }
    }
    return;
}

    bool canAttack = !toAttack().empty();
    bool canDefend = toDefend().size() > 1;

    if (canDefend) {
        auto defendList = toDefend();
        Territory* src = defendList.back();
        Territory* tgt = defendList.front();
        player_->getOrders()->addOrder(new AdvanceOrder(player_, src, tgt, 2));
        std::cout << player_->getName()
                  << " [Human] Defensive Advance " << src->getName()
                  << " -> " << tgt->getName() << "\n";
    }

    if (canAttack) {
        auto defendList = toDefend();
        auto attackList = toAttack();
        if (!defendList.empty() && !attackList.empty()) {
            Territory* src = defendList.front();
            Territory* tgt = attackList.front();
            player_->getOrders()->addOrder(new AdvanceOrder(player_, src, tgt, 2));
            std::cout << player_->getName()
                      << " [Human] Attack " << src->getName()
                      << " -> " << tgt->getName() << "\n";
        }
    }

    if (player_->getHand() && player_->getHand()->size() > 0) {
        player_->getHand()->playCard(0, player_, deck);
    }
}

// ===== AggressivePlayerStrategy =====

AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p)
    : PlayerStrategy(p) {}

std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
    auto terrs = *player_->getTerritories();
    std::sort(terrs.begin(), terrs.end(),
              [](Territory* a, Territory* b){ return a->getArmy() > b->getArmy(); });
    return terrs; // strongest first
}

std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
    std::vector<Territory*> result;
    Territory* strong = strongestTerritory(player_);
    if (!strong) return result;

    for (Territory* adj : strong->getAdjacentTerritories()) {
        if (adj->getPlayer() != player_) result.push_back(adj);
    }
    return result;
}

void AggressivePlayerStrategy::issueOrder(Deck* /*deck*/) {
    // All reinforcements go to strongest territory
    Territory* strong = strongestTerritory(player_);
    if (!strong) return;

    int pool = player_->reinforcementPool();
    if (pool > 0) {
        int deployCount = pool;  // aggressive: dump everything on strongest

        // spend from the pool using your existing API
        if (player_->spendReinforcements(deployCount)) {
            player_->getOrders()->addOrder(
                new DeployOrder(player_, strong, deployCount)
            );

            std::cout << player_->getName() << " [Aggressive] deploys "
                      << deployCount << " to " << strong->getName() << "\n";
        }
        return;
    }

    // Attack from strongest territory to all adjacent enemies (small demo: 3 armies each)
    auto enemies = toAttack();
    for (Territory* tgt : enemies) {
        if (strong->getArmy() <= 1) break;
        int n = std::min(3, strong->getArmy() - 1);
        player_->getOrders()->addOrder(new AdvanceOrder(player_, strong, tgt, n));
        std::cout << player_->getName() << " [Aggressive] attacks "
                  << tgt->getName() << " from " << strong->getName()
                  << " with " << n << " armies\n";
    }
}

// ===== BenevolentPlayerStrategy =====

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* p)
    : PlayerStrategy(p) {}

std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    auto terrs = *player_->getTerritories();
    std::sort(terrs.begin(), terrs.end(),
              [](Territory* a, Territory* b){ return a->getArmy() < b->getArmy(); });
    return terrs; // weakest first
}

std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    // Benevolent never attacks
    return {};
}

void BenevolentPlayerStrategy::issueOrder(Deck* /*deck*/) {
    // Put all reinforcements on weakest territories
    auto terrs = toDefend();          // already sorted weakest → strongest
    if (terrs.empty()) return;

    size_t i = 0;
    // keep going while player still has reinforcements
    while (player_->reinforcementPool() > 0 && !terrs.empty()) {
        Territory* t = terrs[i % terrs.size()];

        // spend 1 army at a time and deploy it
        if (player_->spendReinforcements(1)) {
            player_->getOrders()->addOrder(new DeployOrder(player_, t, 1));
            std::cout << player_->getName()
                      << " [Benevolent] Deploy 1 to " << t->getName() << "\n";
        } else {
            break;  // just in case spendReinforcements fails
        }

        ++i;
    }


    // Optionally move armies from stronger to weaker owned territories
    if (terrs.size() >= 2) {
        Territory* src = terrs.back();   // strongest
        Territory* tgt = terrs.front();  // weakest
        if (src->getArmy() > 1) {
            int n = src->getArmy() / 2;
            player_->getOrders()->addOrder(new AdvanceOrder(player_, src, tgt, n));
        }
    }
}

// ===== NeutralPlayerStrategy =====

NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p)
    : PlayerStrategy(p) {}

std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    return *player_->getTerritories();
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    // Never attacks
    return {};
}

void NeutralPlayerStrategy::issueOrder(Deck* /*deck*/) {
    // Neutral never issues any order
    std::cout << player_->getName() << " [Neutral] issues no orders this turn.\n";
    // NOTE: If you want Neutral → Aggressive when attacked,
    // call player_->setStrategy(new AggressivePlayerStrategy(player_))
    // from AdvanceOrder::execute() when this player is attacked.
}

// ===== CheaterPlayerStrategy =====

CheaterPlayerStrategy::CheaterPlayerStrategy(Player* p)
    : PlayerStrategy(p) {}

std::vector<Territory*> CheaterPlayerStrategy::toDefend() {
    return *player_->getTerritories();
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack() {
    std::vector<Territory*> result;
    for (Territory* t : *player_->getTerritories()) {
        for (Territory* adj : t->getAdjacentTerritories()) {
            if (adj->getPlayer() != player_ &&
                std::find(result.begin(), result.end(), adj) == result.end()) {
                result.push_back(adj);
            }
        }
    }
    return result;
}

void CheaterPlayerStrategy::issueOrder(Deck* /*deck*/) {
    // Automatically conquer all adjacent enemy territories
    auto enemies = toAttack();
    for (Territory* t : enemies) {
        Player* oldOwner = t->getPlayer();
        if (oldOwner && oldOwner != player_) {
            // Remove from previous owner list
            auto* vec = oldOwner->getTerritories();
            vec->erase(std::remove(vec->begin(), vec->end(), t), vec->end());
        }
        t->setPlayer(player_);
        player_->addTerritory(t);
        std::cout << player_->getName()
                  << " [Cheater] instantly conquers " << t->getName() << "\n";
    }
}

// ===== Part 1 driver =====

void testPlayerStrategies() {
    std::cout << "===== testPlayerStrategies() =====\n";

    // Minimal map with 3 territories in a chain
    std::string a = "A", b = "B", c = "C";
    Territory* A = new Territory(a, 0, 0, 0);
    Territory* B = new Territory(b, 1, 0, 0);
    Territory* C = new Territory(c, 2, 0, 0);

    A->addAdjacentTerritory(B);
    B->addAdjacentTerritory(A);
    B->addAdjacentTerritory(C);
    C->addAdjacentTerritory(B);

    Player* human      = new Player("Human");
    Player* aggressive = new Player("Aggressive");
    Player* benevolent = new Player("Benevolent");
    Player* neutral    = new Player("Neutral");
    Player* cheater    = new Player("Cheater");

    // assign basic territories
    A->setPlayer(human);
    B->setPlayer(aggressive);
    C->setPlayer(cheater);

    human->addTerritory(A);
    aggressive->addTerritory(B);
    cheater->addTerritory(C);

    // set strategies
    human->setStrategy(new HumanPlayerStrategy(human));
    aggressive->setStrategy(new AggressivePlayerStrategy(aggressive));
    benevolent->setStrategy(new BenevolentPlayerStrategy(benevolent));
    neutral->setStrategy(new NeutralPlayerStrategy(neutral));
    cheater->setStrategy(new CheaterPlayerStrategy(cheater));

    Deck deck(10);

    std::cout << "\n--- Human turn ---\n";
    human->issueOrder(&deck);

    std::cout << "\n--- Aggressive turn ---\n";
    aggressive->issueOrder(&deck);

    std::cout << "\n--- Benevolent turn ---\n";
    benevolent->issueOrder(&deck);

    std::cout << "\n--- Neutral turn ---\n";
    neutral->issueOrder(&deck);

    std::cout << "\n--- Cheater turn ---\n";
    cheater->issueOrder(&deck);

    delete human;
    delete aggressive;
    delete benevolent;
    delete neutral;
    delete cheater;
    delete A; delete B; delete C;
}
