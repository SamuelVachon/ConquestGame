#include "CommandProcessing.h"
#include <iostream>

/**
 * Simple test driver required in Part 1.
 * Demonstrates: console path, file path, and invalid commands.
 */
void testCommandProcessor() {
    std::cout << "=== Part 1: CommandProcessor (console) ===\n";
    {
        CommandProcessor cp;
        std::cout << "Type a few commands, e.g.:\n"
                     "  loadmap world.map\n"
                     "  validatemap\n"
                     "  addplayer Alice\n"
                     "  gamestart\n"
                     "Type :q to exit this section.\n";
        while (true) {
            Command* c = cp.getCommand();
            if (!c) break;
            std::cout << *c << "\n";
            if (cp.getState() == GameState::ExitProgram) break;
        }
        std::cout << "Final state: " << CommandProcessor::stateToString(cp.getState()) << "\n\n";
    }

    std::cout << "=== Part 1: FileCommandProcessorAdapter (file) ===\n";
    {
        FileCommandProcessorAdapter fcp("resources/commands.txt");
        while (true) {
            Command* c = fcp.getCommand();
            if (!c) break;
            std::cout << *c << "\n";
            if (fcp.getState() == GameState::ExitProgram) break;
        }
        std::cout << "Final state: " << CommandProcessor::stateToString(fcp.getState()) << "\n\n";
    }

    std::cout << "=== Part 1: Invalid command demo (file) ===\n";
    {
        FileCommandProcessorAdapter demo("resources/invalid_demo.txt");
        for (Command* c = nullptr; (c = demo.getCommand()); ) {
            std::cout << *c << "\n";
        }
        std::cout << "Done.\n";
    }
}
