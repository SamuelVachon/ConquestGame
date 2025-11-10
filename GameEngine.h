#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>
#include <iosfwd>
#include "LoggingObserver.h"   // for Subject + ILoggable

class Map;
class MapLoader;
class Player;
class Deck;

class GameEngine : public Subject, public ILoggable {  // loggable subject
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

    // ===== Ctors / dtor / copy
    GameEngine();
    GameEngine(const GameEngine& other);
    GameEngine& operator=(const GameEngine& other);
    ~GameEngine();

    // ===== Top-level loop
    void runConsole();

    // ===== State helpers
    State getState() const;
    std::string stateName() const;
    void transition(State newState);                
    std::string stringToLog() const override;       

    // ===== Commands / FSM
    bool handleCommand(const std::string& cmd, const std::string& arg);
    bool cmd_loadmap(const std::string& filename);
    bool cmd_validatemap();
    bool cmd_addplayer(const std::string& name);
    bool cmd_assigncountries();
    bool cmd_issueorder();
    bool cmd_endissueorders();
    bool cmd_executeorders();
    bool cmd_endexecorders();
    bool cmd_play();
    bool cmd_win();
    bool cmd_end();

    // ===== Helpers
    void resetMapAndCountries();
    void assignTerritoriesRoundRobin();

    friend std::ostream& operator<<(std::ostream& os, const GameEngine& ge);

private:
    // pointers per assignment rules
    State* state_;
    MapLoader* mapLoader_;
    Map* map_;
    std::vector<Player*>* players_;
    Deck* deck_;
    bool* countriesAssigned_;

    // copy helpers
    void deepCopyFrom(const GameEngine& other);
    void clearAll();
};

#endif 

