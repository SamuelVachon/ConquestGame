#include "GameEngine.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>

// Include other team parts so GameEngine can use them
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Card.h"

// ============================
//   CONSTRUCTORS & DESTRUCTOR
// ============================

GameEngine::GameEngine()
: state_(new State(State::Start)),
  mapLoader_(new MapLoader()),
  map_(nullptr),
  players_(new std::vector<Player*>()),
  deck_(nullptr),
  countriesAssigned_(new bool(false)) {
    std::cout << "GameEngine created (state = start)\n";
}

GameEngine::GameEngine(const GameEngine& other)
: state_(nullptr), mapLoader_(nullptr), map_(nullptr), players_(nullptr),
  deck_(nullptr), countriesAssigned_(nullptr) {
    deepCopyFrom(other);
}

GameEngine& GameEngine::operator=(const GameEngine& other) {
    if (this != &other) {
        clearAll();
        deepCopyFrom(other);
    }
    return *this;
}

GameEngine::~GameEngine() {
    clearAll();
    std::cout << "GameEngine destroyed.\n";
}

// ============================
//   DEEP COPY / MEMORY CLEANUP
// ============================

void GameEngine::deepCopyFrom(const GameEngine& other) {
    state_ = new State(*other.state_);
    mapLoader_ = new MapLoader(*other.mapLoader_);
    map_ = nullptr;  // We don't clone the map itself
    players_ = new std::vector<Player*>();
    for (auto* p : *other.players_) {
        players_->push_back(new Player(*p));   // use Player copy constructor
    }
    deck_ = nullptr;
    countriesAssigned_ = new bool(*other.countriesAssigned_);
}

void GameEngine::clearAll() {
    if (players_) {
        for (auto* p : *players_) delete p;
        delete players_;
    }
    delete map_;
    delete mapLoader_;
    delete deck_;
    delete countriesAssigned_;
    delete state_;

    players_ = nullptr;
    map_ = nullptr;
    mapLoader_ = nullptr;
    deck_ = nullptr;
    countriesAssigned_ = nullptr;
    state_ = nullptr;
}

// ============================
//        STATE HELPERS
// ============================

GameEngine::State GameEngine::getState() const { return *state_; }

std::string GameEngine::stateName() const {
    switch (*state_) {
        case State::Start:            return "start";
        case State::MapLoaded:        return "maploaded";
        case State::MapValidated:     return "mapvalidated";
        case State::PlayersAdded:     return "playersadded";
        case State::AssignCountries:  return "assigncountries";
        case State::IssueOrders:      return "issueorders";
        case State::ExecuteOrders:    return "executeorders";
        case State::Win:              return "win";
        case State::End:              return "end";
    }
    return "unknown";
}


// Centralize state changes + notify the observer
void GameEngine::transition(State newState) {
    *state_ = newState;
    notify();   // <- triggers LogObserver to write "STATE <name>"
}

// What the logger writes after each transition()
std::string GameEngine::stringToLog() const {
    std::ostringstream os;
    os << "[GameEngine] state -> " << stateName();
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const GameEngine& ge) {
    os << "[GameEngine state=" << ge.stateName()
       << " players=" << (ge.players_ ? ge.players_->size() : 0)
       << "]";
    return os;
}

// ============================
//     MAIN CONSOLE LOOP
// ============================

void GameEngine::runConsole() {
    std::cout << "=== Warzone Game Engine ===\n";
    std::cout << "Commands: loadmap <file>, validatemap, addplayer <name>,\n"
                 "assigncountries, issueorder, endissueorders, executeorders,\n"
                 "endexecorders, play, win, end\n";
    std::cout << "---------------------------------------------\n";

    std::string line;
    while (*state_ != State::End && std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string cmd, arg;
        iss >> cmd;
        std::getline(iss, arg);
        if (!arg.empty() && arg.front() == ' ') arg.erase(arg.begin());

        bool ok = handleCommand(cmd, arg);
        std::cout << (ok ? "[OK] " : "[ERROR] ")
                  << "New state → " << stateName() << "\n";

        if (*state_ == State::End) break;
    }
}

// ============================
//   COMMAND HANDLER / FSM
// ============================

bool GameEngine::handleCommand(const std::string& cmd, const std::string& arg) {
    switch (*state_) {
        case State::Start:
            if (cmd == "loadmap") return cmd_loadmap(arg);
            break;

        case State::MapLoaded:
            if (cmd == "validatemap") return cmd_validatemap();
            if (cmd == "loadmap")     return cmd_loadmap(arg); // reload allowed
            break;

        case State::MapValidated:
            if (cmd == "addplayer")  return cmd_addplayer(arg);
            if (cmd == "loadmap")     return cmd_loadmap(arg);
            break;

        case State::PlayersAdded:
            if (cmd == "addplayer")   return cmd_addplayer(arg);
            if (cmd == "assigncountries") return cmd_assigncountries();
            break;

        case State::AssignCountries:
            if (cmd == "gamestart") {
                return cmd_gamestart();
            }
            break;

        case State::IssueOrders:
            if (cmd == "issueorder" || cmd == "issueorders") return cmd_issueorder();
            if (cmd == "endissueorders") return cmd_endissueorders();
            break;

        case State::ExecuteOrders:
            if (cmd == "executeorders" || cmd == "execorder") return cmd_executeorders();
            if (cmd == "endexecorders") return cmd_endexecorders();
            if (cmd == "win") return cmd_win();
            break;

        case State::Win:
            if (cmd == "end") return cmd_end();
            if (cmd == "play") { transition(State::IssueOrders); return true; }
            break;

        case State::End:
            return false;
    }

    std::cout << "Invalid command '" << cmd << "' for state '" << stateName() << "'.\n";
    return false;
}

// ============================
//     COMMAND IMPLEMENTATION
// ============================

bool GameEngine::cmd_loadmap(const std::string& filename) {
    if (filename.empty()) {
        std::cout << "Usage: loadmap <conquest-map-file>\n";
        return false;
    }

    resetMapAndCountries();

    std::string f = filename;
    Map* m;
    try {
        m = mapLoader_->loadMap(f);
    } catch (int) {
        std::cout << "The file is not valid" << std::endl;
        return false;
    }

    if (!m->validate()) {
        std::cout << "The map is not valid, try again.\n" << std::endl;
        return false;
    }

    map_ = m;
    transition(State::MapLoaded);               // <-- was *state_ = ...
    std::cout << "Map successfully loaded.\n";
    return true;
}

bool GameEngine::cmd_validatemap() {
    if (!map_) {
        std::cout << "No map loaded yet.\n";
        return false;
    }

    if (map_->validate()) {
        transition(State::MapValidated);        // <-- was *state_ = ...
        std::cout << "Map validated successfully.\n";
        return true;
    } else {
        std::cout << "Map validation failed.\n";
        return false;
    }
}

bool GameEngine::cmd_addplayer(const std::string& name) {
    if (name.empty()) {
        std::cout << "Usage: addplayer <playerName>\n";
        return false;
    }

    players_->push_back(new Player(name));

    if (players_->size() == 2){
        *state_ = State::PlayersAdded;
    }
    if(players_->size() ==6){
        std::cout << "Maximum number of players reached (6). Please proceed to assign countries.\n";
    }
    if (players_->size() > 6){
        std::cout << "Maximum number of players reached (6).\n";
        players_->pop_back(); // remove last added player
        return false;
    }

    std::cout << "Player '" << name << "' added.\n";
    return true;
}

bool GameEngine::cmd_assigncountries() {
    if (!map_) {
        std::cout << "No map to assign.\n";
        return false;
    }

    if (players_->size() < 2) {
        std::cout << "Need at least 2 players to assign countries.\n";
        return false;
    }

    assignTerritoriesRoundRobin();
    *countriesAssigned_ = true;
    transition(State::AssignCountries);         // <-- was *state_ = ...
    std::cout << "Territories assigned in round-robin.\n";
    return true;
}

bool GameEngine::cmd_gamestart() {
    if (!*countriesAssigned_) {
        std::cout << "You must assign countries first.\n";
        return false;
    }

    deck_ = new Deck(52); // create a new deck for the game

    for (auto* p : *players_) {
        Hand* hand = new Hand();
        hand->addCard(deck_->draw());
        hand->addCard(deck_->draw());
        p->setHand(hand);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(players_->begin(), players_->end(), g);

    *state_ = State::IssueOrders;
    std::cout << "Game started! Moving to Issue Orders phase.\n";
    return true;
}

bool GameEngine::cmd_issueorder() {
    if (!*countriesAssigned_) {
        std::cout << "You must assign countries first.\n";
        return false;
    }

    for (auto* p : *players_) {
        p->issueOrder();
    }

    transition(State::IssueOrders);             // <-- was *state_ = ...
    std::cout << "Each player issued one order.\n";
    return true;
}

bool GameEngine::cmd_endissueorders() {
    transition(State::ExecuteOrders);           // <-- was *state_ = ...
    std::cout << "Moving to Execute Orders phase.\n";
    return true;
}

bool GameEngine::cmd_executeorders() {
    std::cout << "Executing all orders...\n";

    for (auto* p : *players_) {
        OrdersList* ol = p->getOrders();
        if (!ol) continue;

        int n = ol->size();
        for (int i = 0; i < n; ++i) {
            auto* o = ol->getOrder(i);
            if (o) o->execute();
        }
    }

    transition(State::ExecuteOrders);           // <-- keep phase consistent & logged
    std::cout << "All orders executed.\n";
    return true;
}

bool GameEngine::cmd_endexecorders() {
    // Doesn’t end game; players can go back to play
    std::cout << "End of execution phase. Type 'play' to continue or 'win' if game is over.\n";
    return true;
}

bool GameEngine::cmd_play() {
    transition(State::IssueOrders);             // <-- was *state_ = ...
    std::cout << "Back to issuing orders.\n";
    return true;
}

bool GameEngine::cmd_win() {
    transition(State::Win);                     // <-- was *state_ = ...
    std::cout << "A player has won! 🎉\n";
    return true;
}

bool GameEngine::cmd_end() {
    transition(State::End);                     // <-- was *state_ = ...
    std::cout << "Game ended. Goodbye!\n";
    return true;
}

// ============================
//        GAME LOOP
// ============================
void GameEngine::reinforcementPhase(){
    std::cout << "\n=== Reinforcement Phase ===" << std::endl;
    for (auto pIt = players_->begin(); pIt != players_->end(); ++pIt) {
        Player* p = *pIt;

        // base reinforcement = max(3, number of territories / 3)
        int terrCount = static_cast<int>(p->getTerritories()->size());
        int baseReinforcements = std::max(3, terrCount / 3);

        int bonus = 0;

        // check continents for ownership
        //Bonus is decided by wether player owns ALL the territories in it's continent
        std::vector<Continent*> continents = map_->getContinents();
        for (auto cIt = continents.begin(); cIt != continents.end(); ++cIt) {
            Continent* c = *cIt;
            bool ownsAll = true;

            std::vector<Territory*> territories = c->getTerritoriesPtr();
            for (auto tIt = territories.begin(); tIt != territories.end(); ++tIt) {
                Territory* t = *tIt;
                if (t->getPlayer() != p) {
                    ownsAll = false;
                    break;
                }
            }
            if (ownsAll) {
                //Number is bonus value(visible in any .map file)
                bonus += c->getNumber();
            }
        }
        int total = baseReinforcements + bonus;
        p->addReinforcements(total);
        std::cout << p->getName() << " receives " << total << " armies." << std::endl;
    }
};

void GameEngine::issueOrdersPhase(){
    std::cout << "\n=== Issuing Orders ===" << std::endl;
    int countDoneIssuing = 0;
    while(countDoneIssuing != players_->size()){
        for (auto pIt = players_->begin(); pIt != players_->end(); ++pIt) {
            Player* p = *pIt;
            if (!p->isDoneIssuing()) {
                p->issueOrder(deck_);
            }else{
                countDoneIssuing++;
            }
        }
    }
};

void GameEngine::executeOrdersPhase(){
    std::cout << "\n=== Executing Orders ===" << std::endl;
    for (auto pIt = players_->begin(); pIt != players_->end(); ++pIt) {
        Player* p = *pIt;
        std::cout << "\n=== Executing " << p->getName() << " orders ===" << std::endl;

        OrdersList* orders = p->getOrders();
        if (!orders) continue;

        // --- Step 1: Execute all Deploy Orders ---
        for (int i = 0; i < orders->size(); ++i) {
            Order* o = orders->getOrder(i);
            if (!o) continue;

            if (o->getDescription() == "Deploy Order") {
                std::cout << "\n>>> Executing Deploy Order #" << i << std::endl;
                o->execute();
            }
        }

        // --- Step 2: Execute all non-Deploy Orders ---
        for (int i = 0; i < orders->size(); ++i) {
            Order* o = orders->getOrder(i);
            if (!o) continue;

            if (o->getDescription() != "Deploy Order") {
                std::cout << "\n>>> Executing Non-Deploy Order #" << i << std::endl;
                o->execute();
            }
        }

        // --- Step 3: Cleanup (remove all executed orders safely) ---
        while (orders->size() > 0) {
            orders->removeOrder(0);
        }
    }

    auto it = players_->begin();
    while(it != players_->end()){
        Player* p = *it;
        if (p->getTerritories()->empty()) {
            std::cout << "\n=== " << p->getName() << " is out of the game! (No territories left) ===" << std::endl;
            delete p;
            it = players_->erase(it);
        }else{
            ++it;
        }
    }
};

void GameEngine::mainGameLoop(){
    std::cout << "\n=== Starting the game... ===" << std::endl;

    if(players_->size()>1){
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
    }
    std::cout << "\n=== GAME OVER! ===" << std::endl;
    if(!players_->empty()){
        std::cout << "Winner: " << players_->front()->getName() << std::endl;
    }
};


// ============================
//        HELPER METHODS
// ============================

void GameEngine::resetMapAndCountries() {
    delete map_;
    map_ = nullptr;
    *countriesAssigned_ = false;
}

void GameEngine::assignTerritoriesRoundRobin() {
    // Algorithm that assigne player a continous set of territory
    auto territories = map_->getTerritories(); // vector<Territory*>
    if (territories.empty()) return;

    size_t idxT =0;
    size_t idxTerr =1;
    size_t idxP =1;
    size_t nTerritories = territories.size();
    size_t nPlayers = players_->size();
    Territory* t = territories[0];
    Territory* temp;
    Territory* temp2;
    Territory* temp3;
    std::vector<Player*> players = *players_;
    bool assigned = false;

    players[0]->addTerritory(t); // first territory to first player
    Territory* toAdd = territories[t->getEdges()[idxT++]];
    temp2 = toAdd;

    for (auto* p : players) {
        for(auto* terr : territories){  
            while(p->toDefend().size() <= ((nTerritories / nPlayers)*idxP)){
                if(terr->getPlayer() == nullptr){
                    p->addTerritory(terr);
                }
                idxTerr++;
                if (idxTerr >= nTerritories-1){
                    break;
                }
            }
        }
        idxTerr=1;
        idxP++;
    }
}

void GameEngine::startupPhase() {
    std::cout << "=== Warzone Game Engine ===\n";
    std::cout << "Commands: loadmap <file>, validatemap, addplayer <name>,\n"
                 "assigncountries, gamestart,  issueorder, endissueorders, executeorders,\n"
                 "endexecorders, play, win, end\n";
    std::cout << "---------------------------------------------\n";

    std::string line;
    while (*state_ != State::IssueOrders) {
        std::string cmd, arg, line;
        std::cout << ">";
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::getline(iss, cmd, ' ');
        std::getline(iss, arg);
        this->handleCommand(cmd, arg);
    }
    std::cout << map_ << std::endl;
    for (auto* p : *players_) {
        std::cout << *p << std::endl;
    }

}

