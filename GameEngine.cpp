#include "GameEngine.h"
#include <iostream>
#include <sstream>
#include <algorithm>

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

// Centralize all state updates here so we also log via notify()
void GameEngine::transition(State newState) {
    *state_ = newState;
    notify();                 // <-- LogObserver::update(...)
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
            if (cmd == "addplayer")   return cmd_addplayer(arg);
            if (cmd == "loadmap")     return cmd_loadmap(arg);
            break;

        case State::PlayersAdded:
            if (cmd == "addplayer")   return cmd_addplayer(arg);
            if (cmd == "assigncountries") return cmd_assigncountries();
            break;

        case State::AssignCountries:
            if (cmd == "issueorder" || cmd == "issueorders") {
                transition(State::IssueOrders);
                return true;
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
    transition(State::PlayersAdded);            // <-- was *state_ = ...
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
//        HELPER METHODS
// ============================

void GameEngine::resetMapAndCountries() {
    delete map_;
    map_ = nullptr;
    *countriesAssigned_ = false;
}

void GameEngine::assignTerritoriesRoundRobin() {
    auto territories = map_->getTerritories(); // vector<Territory*>
    if (territories.empty()) return;

    size_t idx = 0;
    for (auto* t : territories) {
        Player* owner = players_->at(idx % players_->size());
        owner->addTerritory(t);
        ++idx;
    }
}
