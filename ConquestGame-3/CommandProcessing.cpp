#include "CommandProcessing.h"
#include "LoggingObserver.h"
#include <sstream>
#include <cctype>

// ==================== Command ====================
Command::Command() : raw_(new std::string("")), effect_(new std::string("")) {}
Command::Command(const std::string& raw) : raw_(new std::string(raw)), effect_(new std::string("")) {}
Command::Command(const Command& other) : raw_(new std::string(*other.raw_)), effect_(new std::string(*other.effect_)) {}
Command& Command::operator=(const Command& other) {
    if (this != &other) {
        *raw_ = *other.raw_;
        *effect_ = *other.effect_;
    }
    return *this;
}
Command::~Command() { delete raw_; delete effect_; }

void Command::saveEffect(const std::string& eff) {
    *effect_ = eff;     
    notify();           // Part 5 addition (assign2)
}
const std::string& Command::getRaw() const { return *raw_; }
const std::string& Command::getEffect() const { return *effect_; }

std::string Command::stringToLog() const {
    return "Effect:  " + getEffect();
}

std::ostream& operator<<(std::ostream& os, const Command& c) {
    os << "Command{raw=\"" << *c.raw_ << "\", effect=\"" << *c.effect_ << "\"}";
    return os;
}

// ==================== CommandProcessor ====================
CommandProcessor::CommandProcessor()
    : history_(new std::vector<Command*>()),
      state_(new GameState(GameState::Start)) {}

CommandProcessor::CommandProcessor(const CommandProcessor& other)
    : history_(new std::vector<Command*>()),
      state_(new GameState(*other.state_)) {
    for (auto* c : *other.history_) history_->push_back(new Command(*c));
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        for (auto* c : *history_) delete c;
        history_->clear();
        for (auto* c : *other.history_) history_->push_back(new Command(*c));
        *state_ = *other.state_;
    }
    return *this;
}

CommandProcessor::~CommandProcessor() {
    for (auto* c : *history_) delete c;
    delete history_;
    delete state_;
}

Command* CommandProcessor::getCommand() {
    std::string raw = readCommand();
    if (raw.empty()) return nullptr; 
    auto* cmd = new Command(raw);
    saveCommand(cmd);
    validate(cmd);
    return cmd;
}

bool CommandProcessor::validate(Command* cmd) {
    std::istringstream iss(cmd->getRaw());
    std::string name; iss >> name;
    std::string arg; std::getline(iss, arg);
    if (!arg.empty() && arg.front() == ' ') arg.erase(0, 1);

    auto s = *state_;
    auto invalid = [&](const std::string& reason){
        cmd->saveEffect("Invalid: " + reason + " in state " + stateToString(s));
        return false;
    };
    auto ok = [&](const std::string& effect, GameState next){
        cmd->saveEffect(effect + " -> " + stateToString(next));
        setState(next);
        return true;
    };

    for (auto& ch : name) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));

    if (name == "loadmap") {
        if (s == GameState::Start || s == GameState::MapLoaded) {
            if (arg.empty()) return invalid("missing <mapfile> for loadmap");
            return ok("Map loaded: " + arg, GameState::MapLoaded);
        }
        return invalid("loadmap not allowed");
    }
    if (name == "validatemap") {
        if (s == GameState::MapLoaded) {
            return ok("Map validated", GameState::MapValidated);
        }
        return invalid("validatemap not allowed");
    }
    if (name == "addplayer") {
        if (s == GameState::MapValidated || s == GameState::PlayersAdded) {
            if (arg.empty()) return invalid("missing <playername> for addplayer");
            return ok("Player added: " + arg, GameState::PlayersAdded);
        }
        return invalid("addplayer not allowed");
    }
    if (name == "gamestart") {
        if (s == GameState::PlayersAdded) {
            return ok("Startup complete", GameState::AssignReinforcement);
        }
        return invalid("gamestart not allowed");
    }
      // === Assignment 3: tournament command ===
    if (name == "tournament") {
        // Only allowed in start state
        if (s != GameState::Start) {
            return invalid("tournament only allowed in start state");
        }
        if (arg.empty()) {
            return invalid("missing parameters for tournament");
        }

        std::istringstream args(arg);
        std::string token;

        std::vector<std::string> maps;
        std::vector<std::string> strategies;
        int G = -1;
        int D = -1;

        auto parseCsv = [](const std::string& csv) {
            std::vector<std::string> out;
            std::string item;
            std::stringstream ss(csv);
            while (std::getline(ss, item, ',')) {
                if (!item.empty()) out.push_back(item);
            }
            return out;
        };

        while (args >> token) {
            if (token == "-M") {
                std::string list;
                if (!(args >> list)) {
                    return invalid("missing map list after -M");
                }
                maps = parseCsv(list);
            } else if (token == "-P") {
                std::string list;
                if (!(args >> list)) {
                    return invalid("missing strategy list after -P");
                }
                strategies = parseCsv(list);
            } else if (token == "-G") {
                if (!(args >> G)) {
                    return invalid("missing number after -G");
                }
            } else if (token == "-D") {
                if (!(args >> D)) {
                    return invalid("missing number after -D");
                }
            } else {
                return invalid("unexpected token \"" + token + "\" in tournament command");
            }
        }

        // Check all required pieces are present
        if (maps.empty() || strategies.empty() || G < 0 || D < 0) {
            return invalid("missing one or more of -M, -P, -G, -D");
        }

        // Validate ranges (as per assignment)
        if (maps.size() < 1 || maps.size() > 5) {
            return invalid("M must be between 1 and 5 maps");
        }
        if (strategies.size() < 2 || strategies.size() > 4) {
            return invalid("P must be between 2 and 4 player strategies");
        }
        if (G < 1 || G > 5) {
            return invalid("G must be between 1 and 5 games per map");
        }
        if (D < 10 || D > 50) {
            return invalid("D must be between 10 and 50 turns");
        }

        std::ostringstream eff;
        eff << "Tournament accepted: "
            << "M=" << maps.size()
            << " maps, P=" << strategies.size()
            << " strategies, G=" << G
            << ", D=" << D;

        // Stay in Start state (tournament is handled by GameEngine/testTournament)
        cmd->saveEffect(eff.str() + " -> " + stateToString(GameState::Start));
        setState(GameState::Start);
        return true;
    }
    if (name == "replay") {
        if (s == GameState::Win) {
            return ok("Replay requested", GameState::Start);
        }
        return invalid("replay not allowed");
    }
    if (name == "quit") {
        if (s == GameState::Win) {
            cmd->saveEffect("Exiting program");
            setState(GameState::ExitProgram);
            return true;
        }
        return invalid("quit not allowed");
    }

    return invalid("unknown command \"" + name + "\"");
}

const std::vector<Command*>& CommandProcessor::history() const { return *history_; }
GameState CommandProcessor::getState() const { return *state_; }

std::string CommandProcessor::readCommand() {
    std::cout << "[console] enter command> ";
    std::string line;
    if (!std::getline(std::cin, line)) return "";         
    if (line == ":q" || line == "quit" || line == "exit")  
        return "";
    return line;
}


void CommandProcessor::saveCommand(Command* cmd) {
     history_->push_back(cmd); 
     notify();}
void CommandProcessor::setState(GameState s) { *state_ = s; }

std::string CommandProcessor::stateToString(GameState s) {
    switch (s) {
        case GameState::Start: return "start";
        case GameState::MapLoaded: return "maploaded";
        case GameState::MapValidated: return "mapvalidated";
        case GameState::PlayersAdded: return "playersadded";
        case GameState::AssignReinforcement: return "assignreinforcement";
        case GameState::IssueOrders: return "issueorders";
        case GameState::ExecuteOrders: return "executeorders";
        case GameState::Win: return "win";
        case GameState::ExitProgram: return "exit";
    }
    return "unknown";
}

std::string CommandProcessor::stringToLog() const {
    if (history_->empty()) return "No commands";
    return "Command:  " + history_->back()->getRaw();
}
std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp) {
    os << "CommandProcessor{state=" << CommandProcessor::stateToString(*cp.state_) << ", history=[";
    for (size_t i = 0; i < cp.history_->size(); ++i) {
        os << *(cp.history_->at(i));
        if (i + 1 < cp.history_->size()) os << ", ";
    }
    os << "]}";
    return os;
}

// ==================== FileLineReader ====================
FileLineReader::FileLineReader(const std::string& path)
    : in_(new std::ifstream(path)), path_(new std::string(path)) {}

FileLineReader::FileLineReader(const FileLineReader& other)
    : in_(new std::ifstream(*other.path_)), path_(new std::string(*other.path_)) {}

FileLineReader& FileLineReader::operator=(const FileLineReader& other) {
    if (this != &other) {
        delete in_;
        delete path_;
        path_ = new std::string(*other.path_);
        in_ = new std::ifstream(*path_);
    }
    return *this;
}

FileLineReader::~FileLineReader() {
    if (in_->is_open()) in_->close();
    delete in_;
    delete path_;
}

std::string FileLineReader::nextLine() {
    std::string line;
    if (std::getline(*in_, line)) return line;
    return "";
}

bool FileLineReader::good() const { return in_->good(); }

std::ostream& operator<<(std::ostream& os, const FileLineReader& r) {
    os << "FileLineReader{path=\"" << *r.path_ << "\", good=" << std::boolalpha << r.good() << "}";
    return os;
}

// ==================== FileCommandProcessorAdapter (Adapter) ====================
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& path)
    : reader_(new FileLineReader(path)) {}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other)
    : CommandProcessor(other), reader_(new FileLineReader(*other.reader_)) {}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
    if (this != &other) {
        CommandProcessor::operator=(other);
        delete reader_;
        reader_ = new FileLineReader(*other.reader_);
    }
    return *this;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() { delete reader_; }

std::string FileCommandProcessorAdapter::readCommand() {
    if (!reader_->good()) return "";
    return reader_->nextLine();
}

std::ostream& operator<<(std::ostream& os, const FileCommandProcessorAdapter& a) {
    os << "FileCommandProcessorAdapter{reader=" << *(a.reader_) << "}";
    return os;
}
