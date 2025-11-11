#pragma once
/**
 * Part 1 – Command processor & file adapter
 * - All data members of user-defined classes are pointers
 * - Rule of Three implemented where needed
 * - No inline method bodies in the header
 * - Stream insertion operators (operator<<) provided for classes
 */
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Forward declarations
class Command;
class CommandProcessor;
class FileLineReader;
class FileCommandProcessorAdapter;

enum class GameState {
    Start,
    MapLoaded,
    MapValidated,
    PlayersAdded,
    AssignReinforcement,
    IssueOrders,
    ExecuteOrders,
    Win,
    ExitProgram
};

// -------------------- Command --------------------
class Command {
public:
    Command();
    explicit Command(const std::string& raw);
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    void saveEffect(const std::string& eff);
    const std::string& getRaw() const;
    const std::string& getEffect() const;

    friend std::ostream& operator<<(std::ostream& os, const Command& c);

private:
    std::string* raw_;
    std::string* effect_;
};

// -------------------- CommandProcessor --------------------
class CommandProcessor {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);
    CommandProcessor& operator=(const CommandProcessor& other);
    virtual ~CommandProcessor();
    virtual Command* getCommand();
    virtual bool validate(Command* cmd);
    const std::vector<Command*>& history() const;
    GameState getState() const;
    static std::string stateToString(GameState s);
    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp);

protected:
    virtual std::string readCommand();
    void saveCommand(Command* cmd);
    void setState(GameState s);

private:
    std::vector<Command*>* history_;
    GameState* state_;
};

// -------------------- FileLineReader --------------------
class FileLineReader {
public:
    explicit FileLineReader(const std::string& path);
    FileLineReader(const FileLineReader& other);
    FileLineReader& operator=(const FileLineReader& other);
    ~FileLineReader();

    std::string nextLine();
    bool good() const;

    friend std::ostream& operator<<(std::ostream& os, const FileLineReader& r);

private:
    std::ifstream* in_;
    std::string* path_;
};

// -------------------- FileCommandProcessorAdapter (Adapter) --------------------
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    explicit FileCommandProcessorAdapter(const std::string& path);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
    ~FileCommandProcessorAdapter() override;

protected:
    std::string readCommand() override;

public:
    friend std::ostream& operator<<(std::ostream& os, const FileCommandProcessorAdapter& a);

private:
    FileLineReader* reader_;
};

// -------------------- Part 1 driver hook --------------------
void testCommandProcessor();
