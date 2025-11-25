#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"

#include <algorithm>
#include <iostream>
#include <set>

std::vector<Territory*> HumanPlayerStrategy::toDefend(Player* player) {
   
    return player->getTerritories();
}

std::vector<Territory*> HumanPlayerStrategy::toAttack(Player* /*player*/) {
    
    return {};
}

void HumanPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Human] It is player '" << player->getName()
              << "'s turn. (User input would go here.)\n";
}

PlayerStrategy* HumanPlayerStrategy::clone() const { return new HumanPlayerStrategy(*this); }
std::string HumanPlayerStrategy::getName() const { return "Human"; }

std::vector<Territory*> AggressivePlayerStrategy::toDefend(Player* player) {
    // Defend strongest first (descending armies)
    auto territories = player->getTerritories();
    std::sort(territories.begin(), territories.end(),
              [](Territory* a, Territory* b) { return a->getArmies() > b->getArmies(); });
    return territories;
}

std::vector<Territory*> AggressivePlayerStrategy::toAttack(Player* player) {
    // All adjacent enemy territories
    std::vector<Territory*> result;
    std::set<Territory*> seen;

    for (Territory* t : player->getTerritories()) {
        for (Territory* nb : t->getNeighbors()) {
            if (nb->getOwner() != player && !seen.count(nb)) {
                seen.insert(nb);
                result.push_back(nb);
            }
        }
    }
    return result;
}

void AggressivePlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Aggressive] " << player->getName()
              << " would advance from strongest territories into enemies.\n";
}

PlayerStrategy* AggressivePlayerStrategy::clone() const { return new AggressivePlayerStrategy(*this); }
std::string AggressivePlayerStrategy::getName() const { return "Aggressive"; }


std::vector<Territory*> BenevolentPlayerStrategy::toDefend(Player* player) {
    
    auto territories = player->getTerritories();
    std::sort(territories.begin(), territories.end(),
              [](Territory* a, Territory* b) { return a->getArmies() < b->getArmies(); });
    return territories;
}

std::vector<Territory*> BenevolentPlayerStrategy::toAttack(Player* /*player*/) {
    
    return {};
}

void BenevolentPlayerStrategy::issueOrder(Player* player) {
    auto defendList = toDefend(player);
    if (defendList.empty()) {
        std::cout << "[Benevolent] " << player->getName()
                  << " controls no territories and issues no orders.\n";
        return;
    }

    Territory* weakest = defendList.front();
    
    weakest->addArmies(5);

    std::cout << "[Benevolent] " << player->getName()
              << " reinforces weakest territory '"
              << weakest->getName() << "', now with "
              << weakest->getArmies() << " armies.\n";
}

PlayerStrategy* BenevolentPlayerStrategy::clone() const { return new BenevolentPlayerStrategy(*this); }
std::string BenevolentPlayerStrategy::getName() const { return "Benevolent"; }

std::vector<Territory*> NeutralPlayerStrategy::toDefend(Player* player) {
    return player->getTerritories();
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack(Player* /*player*/) {
    return {};
}

void NeutralPlayerStrategy::issueOrder(Player* player) {
    std::cout << "[Neutral] " << player->getName()
              << " stays neutral and issues no orders.\n";
}

PlayerStrategy* NeutralPlayerStrategy::clone() const { return new NeutralPlayerStrategy(*this); }
std::string NeutralPlayerStrategy::getName() const { return "Neutral"; }

std::vector<Territory*> CheaterPlayerStrategy::toDefend(Player* player) {
    return player->getTerritories();
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack(Player* player) {
    // All adjacent enemy territories (unique)
    std::vector<Territory*> result;
    std::set<Territory*> seen;

    for (Territory* t : player->getTerritories()) {
        for (Territory* nb : t->getNeighbors()) {
            if (nb->getOwner() != player && !seen.count(nb)) {
                seen.insert(nb);
                result.push_back(nb);
            }
        }
    }
    return result;
}

void CheaterPlayerStrategy::issueOrder(Player* player) {
    auto attackable = toAttack(player);
    if (attackable.empty()) {
        std::cout << "[Cheater] " << player->getName()
                  << " has no adjacent enemy territories to automatically conquer.\n";
        return;
    }

    std::cout << "[Cheater] " << player->getName()
              << " automatically conquers all adjacent enemies!\n";

    for (Territory* enemy : attackable) {
        Player* oldOwner = enemy->getOwner();
        if (oldOwner && oldOwner != player) {
            oldOwner->removeTerritory(enemy);
        }
        enemy->setOwner(player);
        player->addTerritory(enemy);

        std::cout << "    -> " << enemy->getName()
                  << " is now owned by " << player->getName() << "\n";
    }
}

PlayerStrategy* CheaterPlayerStrategy::clone() const { return new CheaterPlayerStrategy(*this); }
std::string CheaterPlayerStrategy::getName() const { return "Cheater"; }


static void printTerritoryList(const std::string& label,
                               const std::vector<Territory*>& list) {
    std::cout << label;
    if (list.empty()) {
        std::cout << " <none>\n";
        return;
    }
    std::cout << " ";
    for (Territory* t : list) {
        std::cout << t->getName() << "(" << t->getArmies() << ") ";
    }
    std::cout << "\n";
}

void testPlayerStrategies() {
    std::cout << "===== testPlayerStrategies() =====\n";

    // simple mini-map
    Territory t1(1, "Alpha");
    Territory t2(2, "Bravo");
    Territory t3(3, "Charlie");
    Territory t4(4, "Delta");

    // adjacency
    t1.addNeighbor(&t2);
    t2.addNeighbor(&t1);
    t2.addNeighbor(&t3);
    t3.addNeighbor(&t2);
    t3.addNeighbor(&t4);
    t4.addNeighbor(&t3);

    t1.setArmies(2);
    t2.setArmies(8);
    t3.setArmies(4);
    t4.setArmies(1);

    Player benevolentPlayer("BenevolentPlayer", new BenevolentPlayerStrategy());
    Player cheaterPlayer("CheaterPlayer", new CheaterPlayerStrategy());

    // ownership
    t1.setOwner(&benevolentPlayer);
    t2.setOwner(&benevolentPlayer);
    t3.setOwner(&cheaterPlayer);
    t4.setOwner(&cheaterPlayer);

    benevolentPlayer.addTerritory(&t1);
    benevolentPlayer.addTerritory(&t2);
    cheaterPlayer.addTerritory(&t3);
    cheaterPlayer.addTerritory(&t4);

    std::cout << "\n--- Before issuing orders ---\n";
    printTerritoryList("Benevolent toDefend:", benevolentPlayer.toDefend());
    printTerritoryList("Benevolent toAttack:", benevolentPlayer.toAttack());
    printTerritoryList("Cheater toDefend:", cheaterPlayer.toDefend());
    printTerritoryList("Cheater toAttack:", cheaterPlayer.toAttack());

    std::cout << "\n--- Issue orders ---\n";
    benevolentPlayer.issueOrder();
    cheaterPlayer.issueOrder();

    std::cout << "\n--- After issuing orders ---\n";
    std::cout << "Owner of Alpha: "
              << (t1.getOwner() ? t1.getOwner()->getName() : "none") << "\n";
    std::cout << "Owner of Bravo: "
              << (t2.getOwner() ? t2.getOwner()->getName() : "none") << "\n";
    std::cout << "Owner of Charlie: "
              << (t3.getOwner() ? t3.getOwner()->getName() : "none") << "\n";
    std::cout << "Owner of Delta: "
              << (t4.getOwner() ? t4.getOwner()->getName() : "none") << "\n";

    // Dynamic strategy change demo (Neutral -> Aggressive)
    std::cout << "\n--- Dynamic strategy change: Neutral -> Aggressive ---\n";
    Player dynamicPlayer("Switcher", new NeutralPlayerStrategy());
    t1.setOwner(&dynamicPlayer);
    t2.setOwner(&dynamicPlayer);
    dynamicPlayer.addTerritory(&t1);
    dynamicPlayer.addTerritory(&t2);

    dynamicPlayer.issueOrder();
    dynamicPlayer.setStrategy(new AggressivePlayerStrategy());
    dynamicPlayer.issueOrder();
}
