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
    // Starts the game in "start" state
    std::cout << "GameEngine created (state = start)\n";
}

GameEngine::GameEngine(const GameEngine& other)
: state_(nullptr), mapLoader_(nullptr), map_(nullptr), players_(nullptr),
  deck_(nullptr), countriesAssigned_(nullptr) {
    // Copy constructor (deep copy)
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
    // Makes a deep copy of another GameEngine
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
    // Always clean up memory manually (since all members are pointers)
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
    // Converts enum state → readable string
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

std::ostream& operator<<(std::ostream& os, const GameEngine& ge) {
    // Used for printing the GameEngine state
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

        // This function processes the command based on current state
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
    // This is a “state machine”: only certain commands work per state.
    // If a command doesn’t make sense (ex: trying to add a player before loading a map), reject it.

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
                *state_ = State::IssueOrders;
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
            if (cmd == "play") { *state_ = State::IssueOrders; return true; }
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

    resetMapAndCountries(); // delete old map if any

    // MapLoader loads a file and returns a Map*
    std::string f = filename;
    Map* m;
    try{
        m = mapLoader_->loadMap(f);
    }catch(int i){
        std::cout << i;
        std::cout << "The file is not valid" << std::endl;
        return false;
    }

    if (!m->validate()) {
        std::cout << "The map is not valid, try again.\n" << std::endl;
        return false;
    }

    map_ = m;
    *state_ = State::MapLoaded;
    std::cout << "Map successfully loaded.\n";
    return true;
}

bool GameEngine::cmd_validatemap() {
    if (!map_) {
        std::cout << "No map loaded yet.\n";
        return false;
    }

    // Map::validate() checks graph structure & continent rules
    if (map_->validate()) {
        *state_ = State::MapValidated;
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

    // Player has constructor Player(string name)
    players_->push_back(new Player(name));
    *state_ = State::PlayersAdded;
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

    assignTerritoriesRoundRobin(); // evenly distributes territories
    *countriesAssigned_ = true;
    *state_ = State::AssignCountries;
    std::cout << "Territories assigned in round-robin.\n";
    return true;
}

bool GameEngine::cmd_issueorder() {
    if (!*countriesAssigned_) {
        std::cout << "You must assign countries first.\n";
        return false;
    }

    // Ask each player to issue a dummy order
    for (auto* p : *players_) {
        p->issueOrder();
    }

    *state_ = State::IssueOrders;
    std::cout << "Each player issued one order.\n";
    return true;
}

bool GameEngine::cmd_endissueorders() {
    *state_ = State::ExecuteOrders;
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
            if (o) o->execute(); // just prints effects for now
        }
    }

    *state_ = State::ExecuteOrders;
    std::cout << "All orders executed.\n";
    return true;
}

bool GameEngine::cmd_endexecorders() {
    // This doesn’t end the game; players can go back to play
    std::cout << "End of execution phase. Type 'play' to continue or 'win' if game is over.\n";
    return true;
}

bool GameEngine::cmd_play() {
    *state_ = State::IssueOrders;
    std::cout << "Back to issuing orders.\n";
    return true;
}

bool GameEngine::cmd_win() {
    *state_ = State::Win;
    std::cout << "A player has won! 🎉\n";
    return true;
}

bool GameEngine::cmd_end() {
    *state_ = State::End;
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
    // Simple algorithm: assign each territory to a player in sequence.
    auto territories = map_->getTerritories(); // vector<Territory*>
    if (territories.empty()) return;

    size_t idx = 0;
    for (auto* t : territories) {
        Player* owner = players_->at(idx % players_->size());
        owner->addTerritory(t);
        ++idx;
    }
}
