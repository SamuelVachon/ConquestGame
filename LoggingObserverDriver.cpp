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
        notify();                     // -> ORDER-EXECUTED ... :: executed ok
    }
};

void testLoggingObserver() {
    // Writes to gamelog.txt in your LogObserver
    LogObserver* logger = new LogObserver();

    // (2) Create loggable subjects and ATTACH the logger
    GameEngine ge;               ge.attach(logger);      // STATE ...
    TestableCommandProcessor cp; cp.attach(logger);      // COMMAND ...
    Command cmd("loadmap world.map"); cmd.attach(logger);// EFFECT ...
    OrdersList ol;               ol.attach(logger);      // ORDER-ADDED ...
    PingOrder* po = new PingOrder();
    po->attach(logger);                                   // ORDER-EXECUTED ...

    // (3) Trigger COMMAND log via CommandProcessor::saveCommand()
    //     (assignment: log every game command read/saved)
    cp.emitCommand("loadmap world.map");                 // expect a COMMAND line via saveCommand().notify()
    // Optionally demonstrate effect logging comes via Command::saveEffect():
    cmd.saveEffect("map loaded");                        // -> EFFECT map loaded (via Command::saveEffect().notify())

    // (4) Trigger STATE log via GameEngine::transition()
    ge.transition(GameEngine::State::MapLoaded);         // -> STATE MapLoaded (via notify())

    // (5) Trigger ORDER-ADDED via OrdersList::addOrder()
    ol.addOrder(po);                                     // -> ORDER-ADDED Ping

    // (6) Trigger ORDER-EXECUTED via Order::execute()
    //     (your real orders already call notify() inside execute(); our PingOrder does too)
    po->execute();                                       // -> ORDER-EXECUTED Ping :: executed ok

    // (7) Clean up
    delete logger;

    std::cout << "[Part 5] Logging demo done. gamelog.txt for COMMAND/EFFECT/STATE/ORDER lines has been created.\n";
}
