#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Card.h"

#include <algorithm>
#include <iostream>

namespace {
std::vector<Territory*> ownedTerritories(Player* player) {
    if (!player) return {};
    auto terrs = player->getTerritories();
    if (!terrs) return {};
    return *terrs;
}

Territory* strongest(std::vector<Territory*>& terrs) {
    if (terrs.empty()) return nullptr;
    return *std::max_element(
        terrs.begin(), terrs.end(),
        [](Territory* a, Territory* b) { return a->getArmy() < b->getArmy(); });
}

Territory* weakest(std::vector<Territory*>& terrs) {
    if (terrs.empty()) return nullptr;
    return *std::min_element(
        terrs.begin(), terrs.end(),
        [](Territory* a, Territory* b) { return a->getArmy() < b->getArmy(); });
}
} // namespace

// ===== PlayerStrategy base =====
PlayerStrategy::PlayerStrategy(Player* player)
    : player_(player) {}

// ===== Human =====
HumanPlayerStrategy::HumanPlayerStrategy(Player* player)
    : PlayerStrategy(player) {}

std::vector<Territory*> HumanPlayerStrategy::toDefend() {
    return ownedTerritories(player_);
}

std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    auto terrs = ownedTerritories(player_);
    std::reverse(terrs.begin(), terrs.end());
    return terrs;
}

void HumanPlayerStrategy::issueOrder(Deck* /*deck*/) {
    if (!player_) return;
    std::cout << "[Human] " << player_->getName()
              << " would prompt for input and create orders.\n";
}

PlayerStrategy* HumanPlayerStrategy::clone(Player* newOwner) const {
    return new HumanPlayerStrategy(newOwner);
}

std::string HumanPlayerStrategy::getName() const { return "Human"; }

// ===== Aggressive =====
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* player)
    : PlayerStrategy(player) {}

std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
    auto terrs = ownedTerritories(player_);
    std::sort(terrs.begin(), terrs.end(),
              [](Territory* a, Territory* b) { return a->getArmy() > b->getArmy(); });
    return terrs;
}

std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
    auto terrs = ownedTerritories(player_);
    std::reverse(terrs.begin(), terrs.end());
    return terrs;
}

void AggressivePlayerStrategy::issueOrder(Deck* /*deck*/) {
    if (!player_) return;

    auto terrs = ownedTerritories(player_);
    Territory* target = strongest(terrs);
    if (!target) {
        std::cout << "[Aggressive] " << player_->getName()
                  << " controls no territories.\n";
        return;
    }

    int pool = player_->reinforcementPool();
    if (pool > 0 && player_->spendReinforcements(pool)) {
        player_->getOrders()->addOrder(new DeployOrder(player_, target, pool));
        std::cout << "[Aggressive] Deploys " << pool << " to " << target->getName() << ".\n";
    } else {
        // Fallback: still queue an advance placeholder
        player_->getOrders()->addOrder(new AdvanceOrder(player_, target, target, 1));
        std::cout << "[Aggressive] No reinforcements; queues advance from " << target->getName() << ".\n";
    }
}

PlayerStrategy* AggressivePlayerStrategy::clone(Player* newOwner) const {
    return new AggressivePlayerStrategy(newOwner);
}

std::string AggressivePlayerStrategy::getName() const { return "Aggressive"; }

// ===== Benevolent =====
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* player)
    : PlayerStrategy(player) {}

std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    auto terrs = ownedTerritories(player_);
    std::sort(terrs.begin(), terrs.end(),
              [](Territory* a, Territory* b) { return a->getArmy() < b->getArmy(); });
    return terrs;
}

std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    return {};  // benevolent does not attack
}

void BenevolentPlayerStrategy::issueOrder(Deck* /*deck*/) {
    if (!player_) return;

    auto terrs = ownedTerritories(player_);
    Territory* target = weakest(terrs);
    if (!target) {
        std::cout << "[Benevolent] " << player_->getName()
                  << " controls no territories.\n";
        return;
    }

    int pool = player_->reinforcementPool();
    int deploy = std::max(1, pool);
    player_->spendReinforcements(deploy);
    player_->getOrders()->addOrder(new DeployOrder(player_, target, deploy));

    std::cout << "[Benevolent] Reinforces weakest territory " << target->getName()
              << " with " << deploy << " armies.\n";
}

PlayerStrategy* BenevolentPlayerStrategy::clone(Player* newOwner) const {
    return new BenevolentPlayerStrategy(newOwner);
}

std::string BenevolentPlayerStrategy::getName() const { return "Benevolent"; }

// ===== Neutral =====
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* player)
    : PlayerStrategy(player) {}

std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    return ownedTerritories(player_);
}

std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    return {};
}

void NeutralPlayerStrategy::issueOrder(Deck* /*deck*/) {
    if (!player_) return;
    std::cout << "[Neutral] " << player_->getName()
              << " issues no orders.\n";
}

PlayerStrategy* NeutralPlayerStrategy::clone(Player* newOwner) const {
    return new NeutralPlayerStrategy(newOwner);
}

std::string NeutralPlayerStrategy::getName() const { return "Neutral"; }

// ===== Cheater =====
CheaterPlayerStrategy::CheaterPlayerStrategy(Player* player)
    : PlayerStrategy(player) {}

std::vector<Territory*> CheaterPlayerStrategy::toDefend() {
    return ownedTerritories(player_);
}

std::vector<Territory*> CheaterPlayerStrategy::toAttack() {
    auto terrs = ownedTerritories(player_);
    std::reverse(terrs.begin(), terrs.end());
    return terrs;
}

void CheaterPlayerStrategy::issueOrder(Deck* /*deck*/) {
    if (!player_) return;
    // With limited map adjacency info, just double the armies on all owned territories.
    auto terrs = ownedTerritories(player_);
    if (terrs.empty()) {
        std::cout << "[Cheater] " << player_->getName()
                  << " controls no territories.\n";
        return;
    }

    for (Territory* t : terrs) {
        t->setArmy(t->getArmy() * 2);
    }
    std::cout << "[Cheater] Doubles armies on all owned territories.\n";
}

PlayerStrategy* CheaterPlayerStrategy::clone(Player* newOwner) const {
    return new CheaterPlayerStrategy(newOwner);
}

std::string CheaterPlayerStrategy::getName() const { return "Cheater"; }

// ===== Driver =====
static void printTerritoryList(const std::string& label,
                               const std::vector<Territory*>& list) {
    std::cout << label;
    if (list.empty()) {
        std::cout << " <none>\n";
        return;
    }
    std::cout << " ";
    for (Territory* t : list) {
        std::cout << t->getName() << "(" << t->getArmy() << ") ";
    }
    std::cout << "\n";
}

void testPlayerStrategies() {
    std::cout << "===== testPlayerStrategies() =====\n";

    std::string mapFile = "./Maps/Earth.map";
    MapLoader loader;
    Map* map = loader.loadMap(mapFile);
    if (!map) {
        std::cout << "[ERROR] Could not load map " << mapFile << "\n";
        return;
    }

    auto territories = map->getTerritories();
    if (territories.size() < 3) {
        std::cout << "[ERROR] Not enough territories in map to demo strategies.\n";
        delete map;
        return;
    }

    // Set some armies for comparison
    territories[0]->setArmy(2);
    territories[1]->setArmy(6);
    territories[2]->setArmy(1);

    Player aggressive("Aggressive");
    aggressive.setStrategy(new AggressivePlayerStrategy(&aggressive));
    aggressive.addTerritory(territories[0]);
    aggressive.addTerritory(territories[1]);
    aggressive.addReinforcements(5);

    Player benevolent("Benevolent");
    benevolent.setStrategy(new BenevolentPlayerStrategy(&benevolent));
    benevolent.addTerritory(territories[2]);
    benevolent.addReinforcements(3);

    std::cout << "\n--- Before issuing orders ---\n";
    printTerritoryList("Aggressive toDefend:", aggressive.toDefend());
    printTerritoryList("Aggressive toAttack:", aggressive.toAttack());
    printTerritoryList("Benevolent toDefend:", benevolent.toDefend());
    printTerritoryList("Benevolent toAttack:", benevolent.toAttack());

    std::cout << "\n--- Issue orders ---\n";
    aggressive.issueOrder();
    benevolent.issueOrder();

    std::cout << "\n--- Orders queued ---\n";
    std::cout << aggressive.getName() << " orders: " << *aggressive.getOrders() << "\n";
    std::cout << benevolent.getName() << " orders: " << *benevolent.getOrders() << "\n";

    delete map;
}
