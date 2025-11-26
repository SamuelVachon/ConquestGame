#include <iostream>
#include <memory>
#include "LoggingObserver.h"
#include "GameEngine.h"
#include "CommandProcessing.h"
#include "Orders.h"
#include "Player.h"
#include "Map.h"

// ---- Helper: let a driver call the protected saveCommand(...) path
class TestableCommandProcessor : public CommandProcessor {
public:
    void emitCommand(const std::string& raw) {
        // Create a Command and funnel it through the normal save path
        Command* c = new Command(raw);
        // This calls the protected base method (legal from subclass):
        saveCommand(c);                // -> should call notify() inside
    }
};

// ---- Helper concrete Order purely for logging demo
// We keep this tiny to avoid depending on your real map/territories.
class PingOrder : public Order {
public:
    PingOrder() : Order() {
        setDescription("Ping");
        setEffect("constructed");
    }
    // For the demo, validation always succeeds.
    bool validate() override {
        setEffect("validated ok");
        return true;
    }
    void execute() override {
        // produce an effect line and notify the observer
        setEffect("executed ok");
        notify();                     // ORDER-EXECUTED ... :: executed ok
    }
};

void testLoggingObserver() {
    // Writes to gamelog.txt in the LogObserver
    LogObserver* logger = new LogObserver();

    // Create loggable subjects and ATTACH the logger
    GameEngine ge;               ge.attach(logger);      
    TestableCommandProcessor cp; cp.attach(logger);      
    Command cmd("loadmap world.map"); cmd.attach(logger);
    OrdersList ol;               ol.attach(logger);      
    PingOrder* po = new PingOrder();
    po->attach(logger);                                  


    cp.emitCommand("loadmap world.map");                 // expect a COMMAND line (saveCommand().notify())
  
    cmd.saveEffect("map loaded");                        // -> EFFECT map loaded (Command::saveEffect().notify())

    // Trigger STATE log via GameEngine::transition()
    ge.transition(GameEngine::State::MapLoaded);         // -> STATE MapLoaded (notify())

    //  Trigger ORDER-ADDED via OrdersList::addOrder()
    ol.addOrder(po);                                     // -> ORDER-ADDED Ping

    //  Trigger ORDER-EXECUTED via Order::execute()
    po->execute();                                       // -> ORDER-EXECUTED Ping :: executed ok

    // Clean up
    delete logger;
    

    std::cout << "[Part 5] Logging demo done. gamelog.txt for COMMAND/EFFECT/STATE/ORDER lines has been created.\n";
}
