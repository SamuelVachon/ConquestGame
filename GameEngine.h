#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>
#include <memory>
#include <iosfwd>


class Map;
class MapLoader;    // from Map.h
class Player;       // from Player.h
class OrdersList;   // from Orders.h
class Deck;         // from Cards.h
class Hand;         // from Cards.h
class Territory;

class GameEngine {
public:
    enum class State {
        Start,
        MapLoaded,
        MapValidated,
        PlayersAdded,
        AssignCountries,
        IssueOrders,
        ExecuteOrders,
        Win,
        End
    };

    GameEngine();
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);
    ~GameEngine();

    // Driver entry
    void runConsole();  // interactive loop for testGameStates()

    // For testing/automation you can call commands directly:
    bool handleCommand(const std::string& cmd, const std::string& arg = "");

    // Introspection
    State getState() const;
    std::string stateName() const;

private:
    State*               state_;
    MapLoader*           mapLoader_;
    Map*                 map_;
    std::vector<Player*>* players_;
    Deck*                deck_;      
    bool*                countriesAssigned_;

    // Helpers for transitions
    bool cmd_loadmap(const std::string& filename);
    bool cmd_validatemap();
    bool cmd_addplayer(const std::string& name);
    bool cmd_assigncountries();
    bool cmd_issueorder();           // ask each player to issue a dummy order
    bool cmd_endissueorders();       // transition to ExecuteOrders
    bool cmd_executeorders();        // execute all players’ orders (demo)
    bool cmd_endexecorders();        // either Win or back to IssueOrders
    bool cmd_play();                 // back to IssueOrders
    bool cmd_win();
    bool cmd_end();

    // Utilities
    void resetMapAndCountries();
    void clearAll();

    // Deep copy helper
    void deepCopyFrom(const GameEngine& other);

    // Round-robin assignment
    void assignTerritoriesRoundRobin();
};

std::ostream& operator<<(std::ostream& os, const GameEngine& ge);

#endif // GAMEENGINE_H

