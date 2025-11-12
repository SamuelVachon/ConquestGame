#include "CommandProcessing.h"
#include <iostream>
#include <string>

extern void testLoggingObserver();    // from LoggingObserverDriver.cpp
extern void testCommandProcessor();
extern void testOrdersLists();
extern void testMainGameLoop();
extern void testOrderExecution();
extern void startupPhase();
// Run interactive console mode
static int runConsole() {
    CommandProcessor cp;
    std::cout << "[mode] console (type :q to exit)\n";
    while (true) {
        Command* c = cp.getCommand();
        if (!c) break;
        std::cout << *c << "\n";
        if (cp.getState() == GameState::ExitProgram) break;
    }
    return 0;
}

// Run file mode using adapter
static int runFile(const std::string& path) {
    FileCommandProcessorAdapter fcp(path);
    std::cout << "[mode] file: " << path << "\n";
    while (true) {
        Command* c = fcp.getCommand();
        if (!c) break;
        std::cout << *c << "\n";
        if (fcp.getState() == GameState::ExitProgram) break;
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc >= 2) {
        std::string arg1 = argv[1];

        if (arg1 == "-console") return runConsole();

        if (arg1 == "-file") {
            if (argc < 3) { std::cerr << "error: -file requires a filename\n"; return 2; }
            return runFile(argv[2]);
        }

        if (arg1 == "--test") {
            testCommandProcessor();
            testLoggingObserver();
            testOrderExecution();
            testOrdersLists();
            testMainGameLoop();
            return 0;
        }

        // NEW: orders tests
        if (arg1 == "--orders") {        // run both
            testOrdersLists();
            testOrderExecution();
            return 0;
        }
        if (arg1 == "--orders-list") {   // only list behavior
            testOrdersLists();
            return 0;
        }
        if (arg1 == "--orders-exec") {   // only execution rules
            testOrderExecution();
            return 0;
        }
        if (arg1 == "--startup"){
            startupPhase();
            return 0;
        }

        std::cerr << "usage:\n"
                  << "  a2 -console\n"
                  << "  a2 -file <filename>\n"
                  << "  a2 --test\n"
                  << "  a2 --orders            (run both orders tests)\n"
                  << "  a2 --orders-list       (run OrdersList demo)\n"
                  << "  a2 --orders-exec       (run execution demo)\n";
        return 2;
    }
    // Default
    testCommandProcessor();
    testLoggingObserver();
    testOrdersLists();
    testOrderExecution();
    testMainGameLoop();
    startupPhase();
    return 0;
}

