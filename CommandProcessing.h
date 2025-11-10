#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <vector>
#include "LoggingObserver.h"

// Forward
class GameEngine;

class Command : public Subject, public ILoggable {
public:
    Command();
    Command(const std::string& cmd);
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    void saveEffect(const std::string& fx); // must notify

    const std::string& getCmd() const;
    const std::string& getEffect() const;

    std::string stringToLog() const override;

private:
    std::string* cmd_;
    std::string* effect_;
};

class CommandProcessor : public Subject, public ILoggable {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);
    CommandProcessor& operator=(const CommandProcessor& other);
    virtual ~CommandProcessor();

    // For console or adapter to push in a command string
    virtual void saveCommand(const std::string& cmd); // must notify
    // For Part 1 validate logic (stub-friendly)
    virtual bool validate(const std::string& cmd, const GameEngine* ge);

    const std::vector<Command*>& getHistory() const;

    std::string stringToLog() const override;

protected:
    std::vector<Command*>* history_;
};

class FileCommandProcessorAdapter : public CommandProcessor {
public:
    explicit FileCommandProcessorAdapter(const std::string& path);
    ~FileCommandProcessorAdapter() override;

    void loadAll(); // read file and call saveCommand per line

private:
    std::string* path_;
};

#endif // COMMANDPROCESSING_H
