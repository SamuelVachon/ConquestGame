#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include "LoggingObserver.h"
#include "PlayerStrategies.h"
#include <typeinfo>


// Methods for Orders Class
// Getters
 string Order::getDescription() const{
    return *description;
 };

 string Order::getEffect() const{
    return *effect;
 };

 Player* Order::getIssuer() const{
    return issuer;
 };

 Territory* Order::getTarget() const{
    return target;
 };

 // Setters 
 void Order::setDescription(const string& description){
    if(this->description){
        delete this->description;
    }
    this->description=new string(description);
 };

 void Order::setEffect(const string& effect){
    if(this->effect){
        delete this->effect;
    }

    this->effect=new string(effect);
 };

 void Order::setIssuer(Player* issuer) {
    this->issuer = issuer;
};

void Order::setTarget(Territory* target){
    this->target=target;
};

// Default Constructor
Order::Order(){
    this->description=new string("No description");
    this->effect=new string("No effect");
    this->issuer=nullptr;
    this->target=nullptr;
};

// Paramaterized Constructor
Order::Order(const string& description, const string& effect,Player* issuer, Territory* target){
    this->description=new string(description);
    this->effect=new string(effect);
    this->issuer=issuer;
    this->target=target;
};

// Copy Constructor
Order::Order(const Order& other){
    this->description=new string(*other.description);
    this->effect=new string(*other.effect);
    this->issuer=other.issuer;
    this->target=other.target;
};

// Destructor
Order::~Order(){
    delete description;
    delete effect;
};

// Assignment operator
Order& Order::operator=(const Order& other){
    if(this==&other){
        return *this;
    }

    delete description;
    delete effect;

    description=new string(*other.description);
    effect=new string(*other.effect);
    issuer=other.issuer;
    target=other.target;

    return *this;

};

// Stream operator
ostream& operator <<(ostream& os, const Order& order){
    os << "Order: "<< *order.description
       << "| Effect: "<< *order.effect;

    if (order.issuer) {
        os << " | Issuer: [Player exists]";
    } else {
        os << " | Issuer: [None]";
    }

    if (order.target) {
        os << " | Target: [Territory exists]";
    } else {
        os << " | Target: [None]";
    }

    return os;
};
// Assignment 2 part 5 addition:
std::string Order::stringToLog() const {
    // Keep the line simple for the log
    return "ORDER-EXECUTED " + getDescription() + " :: " + getEffect();
}

// Methods for Deploy Order Class

// Getter
int DeployOrder::getNumArmies() const{
    return *numArmies;
}

// Setter
void DeployOrder::setNumArmies(int numArmies){
    if(this->numArmies){
        delete this->numArmies;
    }
    this->numArmies= new int(numArmies);
}

// default Constructor
DeployOrder::DeployOrder(){
    this->numArmies=new int(0);
};

// Paramaterized Constructor
DeployOrder::DeployOrder(Player* issuer, Territory* target, int numArmies)
: Order("Deploy Order", "Pending", issuer, target)
{
    this->numArmies=new int(numArmies);
};

// Copy Constructor
DeployOrder::DeployOrder(const DeployOrder& other){
    this->numArmies=new int(*other.numArmies);
};

// Assignment Operator
DeployOrder& DeployOrder::operator=(const DeployOrder& other){
    if(this==&other){
        return *this;
    }

    delete numArmies;

    this->numArmies=new int(*other.numArmies);

    return *this;
};

// Destructor
DeployOrder::~DeployOrder() {
    delete numArmies;
};

// Validate Method

bool DeployOrder::validate(){
    if (getIssuer() == nullptr) {
    cout << "Invalid: No player assigned.\n";
    return false;
}

if (getTarget() == nullptr) {
    cout << "Invalid: No target assigned.\n";
    return false;
}

// Ownership check player must actually own the target territory
    
    Player* issuer = getIssuer();
    Territory* target = getTarget();

    bool ownsTarget = false;
    for (Territory* terr : issuer->toDefend()) {
        if (terr == target) {
            ownsTarget = true;
            break;
        }
    }

    if (!ownsTarget) {
        std::cout << "Invalid: player " << issuer->getName()
                  << " does not own the target territory "
                  << target->getName() << ".\n";
        return false;
    }

if (*numArmies <= 0) {
    cout << "Invalid: Number of armies must be greater than 0.\n";
    return false;
}

  return true;
};

void DeployOrder::execute() {
    cout << "\n[Executing Deploy Order...]\n";
    if (!validate()) {
        setEffect("Invalid order — deployment failed.");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition
        return;
    }

    Player* p = getIssuer();
    Territory* t = getTarget();

    if (!p->spendReinforcements(*numArmies)) {
        setEffect("Invalid: not enough armies in reinforcement pool.");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition
        return;
    }

    t->setArmy(t->getArmy() + *numArmies);
    setEffect("Deployed " + std::to_string(*numArmies) + " armies to " + t->getName() + ".");
    cout << getEffect() << endl;
    notify();                        // Part 5 addition
}



ostream& operator<<(ostream& os, const DeployOrder& order) {
    os << "\n[DeployOrder]\n";

    // Print basic details
    os << "  Issuer: ";
    if (order.getIssuer())
        os << order.getIssuer()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Target Territory: ";
    if (order.getTarget())
        os << order.getTarget()->getName();
    else
        os << "(none)";
    os << "\n";

    // Print number of armies
    os << "  Number of Armies: ";
    if (order.getNumArmies() > 0)
        os << order.getNumArmies();
    else
        os << "(invalid)";
    os << "\n";

    // Print current effect message (if any)
    os << "  Effect: ";
    if (!order.getEffect().empty())
        os << order.getEffect();
    else
        os << "(none)";
    os << "\n";

    return os;
};


// Methods for Advance Order

// Getter
Territory* AdvanceOrder::getSource() const {
    return source;
};

int AdvanceOrder::getNumArmies() const {
    return *numArmies;
};

// Setter
void AdvanceOrder::setSource(Territory* source) {
    this->source = source;
};

void AdvanceOrder::setNumArmies(int numArmies) {
    if (this->numArmies) {
        delete this->numArmies;
    }
    this->numArmies = new int(numArmies);
};

// Default Constructor
AdvanceOrder::AdvanceOrder()
    : Order("Advance", "Pending", nullptr, nullptr) {
    this->source = nullptr;
    this->numArmies = new int(0);
};

// Parameterized Constructor
AdvanceOrder::AdvanceOrder(Player* issuer, Territory* source, Territory* target, const int& numArmies)
    : Order("Advance", "Pending", issuer, target) {
    this->source = source;
    this->numArmies = new int(numArmies);
};

// Copy Constructor
AdvanceOrder::AdvanceOrder(const AdvanceOrder& other)
    : Order(other) {
    this->source = other.source;
    this->numArmies = new int(*other.numArmies);
};

// Assignment Operator
AdvanceOrder& AdvanceOrder::operator=(const AdvanceOrder& other) {
    if (this == &other) {
        return *this;
    }

    Order::operator=(other);

    this->source = other.source;

    if (this->numArmies) {
        delete this->numArmies;
    }
    this->numArmies = new int(*other.numArmies);

    return *this;
};

// Destructor
AdvanceOrder::~AdvanceOrder() {
    delete numArmies;
    numArmies = nullptr;
};

// Validate method

bool AdvanceOrder::validate() {
    if (getIssuer() == nullptr) {
        cout << "Invalid: No player assigned.\n";
        return false;
    }

    // Negotiation rule: cannot attack if players have a truce
     if (getTarget()->getPlayer() && getIssuer()->hasTruceWith(getTarget()->getPlayer())) {
    cout << "Invalid: players are under negotiation (truce)." << endl;
    return false;
    }

    if (getSource() == nullptr || getTarget() == nullptr) {
        cout << "Invalid: Missing source or target territory.\n";
        return false;
    }

    if (*numArmies <= 0) {
        cout << "Invalid: Number of armies must be greater than 0.\n";
        return false;
    }

    Player* issuer = getIssuer();
    Territory* src = getSource();
    Territory* tgt = getTarget();

    // Check that player owns source
    bool ownsSource = false;
    for (Territory* terr : issuer->toDefend()) {
        if (terr == src) {
            ownsSource = true;
            break;
        }
    }

    if (!ownsSource) {
        cout << "Invalid: Player " << issuer->getName()
             << " does not own the source territory "
             << src->getName() << ".\n";
        return false;
    }

    // Check that source has enough armies
    if (src->getArmy() < *numArmies) {
        cout << "Invalid: Not enough armies in " << src->getName() << ".\n";
        return false;
    }

    // Simple adjacency validation
    vector<string> edgeNames = src->getEdgesNames();
    bool isAdjacent = false;
    for (const string& neighbor : edgeNames) {
        if (neighbor == tgt->getName()) {
            isAdjacent = true;
            break;
        }
    }

    if (!isAdjacent && src != tgt) {
        cout << "Invalid: Target territory " << tgt->getName()
             << " is not adjacent to source " << src->getName() << ".\n";
        return false;
    }

    return true;
};

// Execute Method

void AdvanceOrder::execute() {
    cout << "\n[Executing Advance Order...]\n";
    if (!validate()) {
        setEffect("Invalid order — advance failed.");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition (assign 2)
        return;
    }

    Player* issuer = getIssuer();
    Territory* src = getSource();
    Territory* tgt = getTarget();
    int moving = *numArmies;

    src->setArmy(src->getArmy() - moving);

    if (tgt->getPlayer() == issuer || tgt == src) {
        tgt->setArmy(tgt->getArmy() + moving);
        setEffect("Moved " + to_string(moving) + " armies from " + src->getName() + " to " + tgt->getName() + ".");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition (assign 2)
        return;
    }

    Player* defender = tgt->getPlayer();
    int atk = moving;
    int def = tgt->getArmy();

    srand(static_cast<unsigned>(time(nullptr)));
    while (atk > 0 && def > 0) {
        int killsOnDef = 0;
        for (int i = 0; i < atk; ++i) if ((rand() % 100) < 60) ++killsOnDef;
        def = max(0, def - killsOnDef);
        if (def == 0) break;

        int killsOnAtk = 0;
        for (int i = 0; i < def; ++i) if ((rand() % 100) < 70) ++killsOnAtk;
        atk = max(0, atk - killsOnAtk);
    }

    if (def == 0 && atk > 0) {
        tgt->setPlayer(issuer);
        tgt->setArmy(atk);
        issuer->markConquered();
        setEffect("Conquered " + tgt->getName() + " with " + to_string(atk) + " surviving armies.");
    } else {
        tgt->setArmy(def);
        setEffect("Attack failed on " + tgt->getName() + " (defenders left: " + to_string(def) + ").");
    }
    cout << getEffect() << endl;

    //Change the player type from neutral to aggresive if attacked
    if (std::typeid(getTarget()->getPlayer()->getStrategy()) == typeid(NeutralPlayerStrategy)){
        getTarget()->getPlayer()->setStrategy(new AggressivePlayerStrategy(getTarget()->getPlayer()));
    }
    notify();                        // Part 5 addition (assign 2)
}


// Stream Operator

ostream& operator<<(ostream& os, const AdvanceOrder& order) {
    os << "\n[AdvanceOrder]\n";

    os << "  Issuer: ";
    if (order.getIssuer())
        os << order.getIssuer()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Source Territory: ";
    if (order.getSource())
        os << order.getSource()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Target Territory: ";
    if (order.getTarget())
        os << order.getTarget()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Number of Armies: ";
    if (order.getNumArmies() > 0)
        os << order.getNumArmies();
    else
        os << "(invalid)";
    os << "\n";

    os << "  Effect: ";
    if (!order.getEffect().empty())
        os << order.getEffect();
    else
        os << "(none)";
    os << "\n";

    return os;
};

// CLASS: BombOrder

// Represents a Bomb order: reduces the number of armies in an enemy territory by half

// Default Constructor
BombOrder::BombOrder()
    : Order("Bomb", "Pending", nullptr, nullptr) {};

// Parameterized Constructor
BombOrder::BombOrder(Player* issuer, Territory* target)
    : Order("Bomb", "Pending", issuer, target) {};

// Copy Constructor
BombOrder::BombOrder(const BombOrder& other)
    : Order(other) {};

// Assignment Operator
BombOrder& BombOrder::operator=(const BombOrder& other) {
    if (this == &other) {
        return *this;
    }
    Order::operator=(other);
    return *this;
};

// Destructor
BombOrder::~BombOrder() = default;

// Validate

bool BombOrder::validate() {
    if (getIssuer() == nullptr) {
        cout << "Invalid: No player assigned.\n";
        return false;
    }

    if (getTarget() == nullptr) {
        cout << "Invalid: No target territory assigned.\n";
        return false;
    }

    Player* issuer = getIssuer();
    Territory* target = getTarget();

    // Player cannot bomb their own territory
    for (Territory* terr : issuer->toDefend()) {
        if (terr == target) {
            cout << "Invalid: Player cannot bomb their own territory "
                 << target->getName() << ".\n";
            return false;
        }
    }

    // Player must own at least one adjacent territory to target
    bool isAdjacent = false;
    for (Territory* terr : issuer->toDefend()) {
        vector<string> neighbors = terr->getEdgesNames();
        for (const string& n : neighbors) {
            if (n == target->getName()) {
                isAdjacent = true;
                break;
            }
        }
        if (isAdjacent) break;
    }

    if (!isAdjacent) {
        cout << "Invalid: Target " << target->getName()
             << " is not adjacent to any of the player's territories.\n";
        return false;
    }

    return true;
};

// Execute

void BombOrder::execute() {
    cout << "\n[Executing Bomb Order...]\n";

    if (!validate()) {
        setEffect("Invalid order — bombing failed.");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition (assign 2)
        return;
    }

    Territory* target = getTarget();
    int originalArmies = target->getArmy();
    int destroyedArmies = originalArmies / 2;

    target->setArmy(destroyedArmies);

    setEffect("Bombed " + target->getName() +
              ": enemy armies reduced from " +
              to_string(originalArmies) + " to " + to_string(destroyedArmies) + ".");
    cout << getEffect() << endl;

    //Change the player type from neutral to aggresive if attacked
    if (std::typeid(getTarget()->getPlayer()->getStrategy()) == typeid(NeutralPlayerStrategy)){
        getTarget()->getPlayer()->setStrategy(new AggressivePlayerStrategy(getTarget()->getPlayer()));
    }
    notify();                        // Part 5 addition (assign 2)
}


// Stream Operator

ostream& operator<<(ostream& os, const BombOrder& order) {
    os << "\n[BombOrder]\n";

    os << "  Issuer: ";
    if (order.getIssuer())
        os << order.getIssuer()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Target Territory: ";
    if (order.getTarget())
        os << order.getTarget()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Effect: ";
    if (!order.getEffect().empty())
        os << order.getEffect();
    else
        os << "(none)";
    os << "\n";

    return os;
};

// CLASS: BlockadeOrder
// Represents a Blockade order: triples the armies on a territory and transfers
// ownership to a neutral player.

// Default Constructor
BlockadeOrder::BlockadeOrder()
    : Order("Blockade", "Pending", nullptr, nullptr) {};

// Parameterized Constructor
BlockadeOrder::BlockadeOrder(Player* issuer, Territory* target)
    : Order("Blockade", "Pending", issuer, target) {};

// Copy Constructor
BlockadeOrder::BlockadeOrder(const BlockadeOrder& other)
    : Order(other) {};

// Assignment Operator
BlockadeOrder& BlockadeOrder::operator=(const BlockadeOrder& other) {
    if (this == &other) {
        return *this;
    }
    Order::operator=(other);
    return *this;
};

// Destructor
BlockadeOrder::~BlockadeOrder() = default;


// Validate

bool BlockadeOrder::validate() {
    if (getIssuer() == nullptr) {
        cout << "Invalid: No player assigned.\n";
        return false;
    }

    if (getTarget() == nullptr) {
        cout << "Invalid: No target territory assigned.\n";
        return false;
    }

    Player* issuer = getIssuer();
    Territory* target = getTarget();

    // Player must own the target territory
    bool ownsTarget = false;
    for (Territory* terr : issuer->toDefend()) {
        if (terr == target) {
            ownsTarget = true;
            break;
        }
    }

    if (!ownsTarget) {
        cout << "Invalid: Player " << issuer->getName()
             << " does not own the target territory " 
             << target->getName() << ".\n";
        return false;
    }

    return true;
};

static Player* getNeutralPlayer() {
    static Player neutral("Neutral");
    return &neutral;
}

void BlockadeOrder::execute() {
    cout << "\n[Executing Blockade Order...]\n";
    if (!validate()) {
        setEffect("Invalid order — blockade failed.");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition (assign 2)
        return;
    }

    Territory* t = getTarget();
    int newArmies = t->getArmy() * 2;
    t->setArmy(newArmies);
    t->setPlayer(getNeutralPlayer());
    setEffect("Blockade on " + t->getName() + ": doubled to " + to_string(newArmies) + " and transferred to Neutral.");
    cout << getEffect() << endl;
    notify();                        // Part 5 addition (assign 2)
}


// Stream Operator

ostream& operator<<(ostream& os, const BlockadeOrder& order) {
    os << "\n[BlockadeOrder]\n";

    os << "  Issuer: ";
    if (order.getIssuer())
        os << order.getIssuer()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Target Territory: ";
    if (order.getTarget())
        os << order.getTarget()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Effect: ";
    if (!order.getEffect().empty())
        os << order.getEffect();
    else
        os << "(none)";
    os << "\n";

    return os;
};

// CLASS: AirliftOrder

// Getters

Territory* AirliftOrder::getSource() const {
    return source;
};

int AirliftOrder::getNumArmies() const {
    return *numArmies;
};

// Setters

void AirliftOrder::setSource(Territory* source) {
    this->source = source;
};

void AirliftOrder::setNumArmies(int numArmies) {
    if (this->numArmies) {
        delete this->numArmies;
    }
    this->numArmies = new int(numArmies);
};

// Default Constructor
AirliftOrder::AirliftOrder()
    : Order("Airlift", "Pending", nullptr, nullptr) {
    this->source = nullptr;
    this->numArmies = new int(0);
};

// Parameterized Constructor
AirliftOrder::AirliftOrder(Player* issuer, Territory* source, Territory* target, int numArmies)
    : Order("Airlift", "Pending", issuer, target) {
    this->source = source;
    this->numArmies = new int(numArmies);
};

// Copy Constructor
AirliftOrder::AirliftOrder(const AirliftOrder& other)
    : Order(other) {
    this->source = other.source;
    this->numArmies = new int(*other.numArmies);
};

// Assignment Operator
AirliftOrder& AirliftOrder::operator=(const AirliftOrder& other) {
    if (this == &other)
        return *this;

    Order::operator=(other);
    this->source = other.source;

    if (this->numArmies) {
        delete this->numArmies;
    }
    this->numArmies = new int(*other.numArmies);

    return *this;
};

// Destructor
AirliftOrder::~AirliftOrder() {
    delete numArmies;
    numArmies = nullptr;
};

// Validate

bool AirliftOrder::validate() {
    if (getIssuer() == nullptr) {
        cout << "Invalid: No player assigned.\n";
        return false;
    }

    if (source == nullptr || getTarget() == nullptr) {
        cout << "Invalid: Missing source or target territory.\n";
        return false;
    }

    if (*numArmies <= 0) {
        cout << "Invalid: Number of armies must be greater than 0.\n";
        return false;
    }

    Player* issuer = getIssuer();
    Territory* src = source;
    Territory* tgt = getTarget();

    // Check ownership of source
    bool ownsSource = false;
    for (Territory* terr : issuer->toDefend()) {
        if (terr == src) {
            ownsSource = true;
            break;
        }
    }

    if (!ownsSource) {
        cout << "Invalid: Player does not own source territory "
             << src->getName() << ".\n";
        return false;
    }

    // Check ownership of target
    bool ownsTarget = false;
    for (Territory* terr : issuer->toDefend()) {
        if (terr == tgt) {
            ownsTarget = true;
            break;
        }
    }

    if (!ownsTarget) {
        cout << "Invalid: Player does not own target territory "
             << tgt->getName() << ".\n";
        return false;
    }

    // Check if enough armies are available
    if (src->getArmy() < *numArmies) {
        cout << "Invalid: Not enough armies in source territory.\n";
        return false;
    }

    return true;
};

// Execute

void AirliftOrder::execute() {
    cout << "\n[Executing Airlift Order...]\n";

    if (!validate()) {
        setEffect("Invalid order — airlift failed.");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition (assign 2)
        return;
    }

    Territory* src = source;
    Territory* tgt = getTarget();
    int movingArmies = *numArmies;

    src->setArmy(src->getArmy() - movingArmies);
    tgt->setArmy(tgt->getArmy() + movingArmies);

    setEffect("Airlifted " + to_string(movingArmies) + " armies from " +
              src->getName() + " to " + tgt->getName() + ".");
    cout << getEffect() << endl;
    notify();                        // Part 5 addition (assign 2)
}


// Stream Operator

ostream& operator<<(ostream& os, const AirliftOrder& order) {
    os << "\n[AirliftOrder]\n";

    os << "  Issuer: ";
    if (order.getIssuer())
        os << order.getIssuer()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Source Territory: ";
    if (order.getSource())
        os << order.getSource()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Target Territory: ";
    if (order.getTarget())
        os << order.getTarget()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Number of Armies: ";
    if (order.getNumArmies() > 0)
        os << order.getNumArmies();
    else
        os << "(invalid)";
    os << "\n";

    os << "  Effect: ";
    if (!order.getEffect().empty())
        os << order.getEffect();
    else
        os << "(none)";
    os << "\n";

    return os;
};

// CLASS: NegotiateOrder

// Getter and Setter

Player* NegotiateOrder::getOtherPlayer() const {
    return otherPlayer;
};

void NegotiateOrder::setOtherPlayer(Player* otherPlayer) {
    this->otherPlayer = otherPlayer;
};

// Default Constructor
NegotiateOrder::NegotiateOrder()
    : Order("Negotiate", "Pending", nullptr, nullptr) {
    this->otherPlayer = nullptr;
};

// Parameterized Constructor
NegotiateOrder::NegotiateOrder(Player* issuer, Player* otherPlayer)
    : Order("Negotiate", "Pending", issuer, nullptr) {
    this->otherPlayer = otherPlayer;
};

// Copy Constructor
NegotiateOrder::NegotiateOrder(const NegotiateOrder& other)
    : Order(other) {
    this->otherPlayer = other.otherPlayer;
};

// Assignment Operator
NegotiateOrder& NegotiateOrder::operator=(const NegotiateOrder& other) {
    if (this == &other)
        return *this;

    Order::operator=(other);
    this->otherPlayer = other.otherPlayer;

    return *this;
};

// Destructor
NegotiateOrder::~NegotiateOrder() = default;

// Validate

bool NegotiateOrder::validate() {
    if (getIssuer() == nullptr) {
        cout << "Invalid: No player assigned.\n";
        return false;
    }

    if (otherPlayer == nullptr) {
        cout << "Invalid: No other player assigned.\n";
        return false;
    }

    if (getIssuer() == otherPlayer) {
        cout << "Invalid: Player cannot negotiate with themselves.\n";
        return false;
    }

    return true;
};

// Execute

void NegotiateOrder::execute() {
    cout << "\n[Executing Negotiate Order...]\n";
    if (!validate()) {
        setEffect("Invalid order — negotiation failed.");
        cout << getEffect() << endl;
        notify();                    // Part 5 addition (assign 2)
        return;
    }

    Player* p1 = getIssuer();
    Player* p2 = otherPlayer;
    p1->addTruceWith(p2);
    p2->addTruceWith(p1);

    setEffect("Negotiation established between " + p1->getName() + " and " + p2->getName() + " (no attacks this turn).");
    cout << getEffect() << endl;
    notify();                        // Part 5 addition (assign 2)
}


// Stream Operator

ostream& operator<<(ostream& os, const NegotiateOrder& order) {
    os << "\n[NegotiateOrder]\n";

    os << "  Issuer: ";
    if (order.getIssuer())
        os << order.getIssuer()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Other Player: ";
    if (order.getOtherPlayer())
        os << order.getOtherPlayer()->getName();
    else
        os << "(none)";
    os << "\n";

    os << "  Effect: ";
    if (!order.getEffect().empty())
        os << order.getEffect();
    else
        os << "(none)";
    os << "\n";

    return os;
};

// CLASS: OrdersList

// Default Constructor
OrdersList::OrdersList() {
    this->orders = new vector<Order*>();
};

// Copy Constructor (deep copy)
OrdersList::OrdersList(const OrdersList& other) {
    this->orders = new vector<Order*>();
    for (Order* o : *other.orders) {
        this->orders->push_back(o); 
    }
};

// Assignment Operator (deep copy)
OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this == &other)
        return *this;

    // Delete old orders
    for (Order* o : *orders) {
        delete o;
    }
    orders->clear();

    // Deep copy new ones
    for (Order* o : *other.orders) {
        orders->push_back(o); 
    }

    return *this;
};

// Destructor
OrdersList::~OrdersList() {
    for (Order* o : *orders) {
        delete o;
    }
    delete orders;
    orders = nullptr;
};

// Returns pointer to the Order at a given index
Order* OrdersList::getOrder(int index) const {
    if (index < 0 || index >= static_cast<int>(orders->size())) {
        cout << "Invalid index. Returning nullptr.\n";
        return nullptr;
    }
    return (*orders)[index];
};

// Returns the number of Orders in the list
int OrdersList::size() const {
    return static_cast<int>(orders->size());
};

// Deletes an Order at the specified index
void OrdersList::removeOrder(int index) {
    if (index < 0 || index >= static_cast<int>(orders->size())) {
        cout << "Invalid index. Cannot remove.\n";
        return;
    }

    Order* toRemove = (*orders)[index];
    delete toRemove;
    orders->erase(orders->begin() + index);

    cout << "Order at index " << index << " removed successfully.\n";
};

// Adds a new Order to the list
void OrdersList::addOrder(Order* order) {
    if (order == nullptr) {
        cout << "Cannot add null Order.\n";
        return;
    }
    orders->push_back(order);
    cout << "Order added successfully.\n";

    // Part 5 addition (assign 2)
    lastAdded_ = order;
    notify();
}


// Moves an Order from one position to another
void OrdersList::moveOrder(int fromIndex, int toIndex) {
    if (fromIndex < 0 || fromIndex >= static_cast<int>(orders->size()) ||
        toIndex < 0 || toIndex >= static_cast<int>(orders->size())) {
        cout << "Invalid index(es). Cannot move order.\n";
        return;
    }

    Order* temp = (*orders)[fromIndex];
    orders->erase(orders->begin() + fromIndex);
    orders->insert(orders->begin() + toIndex, temp);

    cout << "Order moved from index " << fromIndex 
         << " to " << toIndex << ".\n";
};

// Part 5 addition (assign 2)
std::string OrdersList::stringToLog() const {
    return std::string("ORDER-ADDED ") + (lastAdded_ ? lastAdded_->getDescription() : "(null)");
}


// Stream Operator

ostream& operator<<(ostream& os, const OrdersList& ol) {
    os << "\n[OrdersList: " << ol.size() << " orders]\n";
    if (ol.size() == 0) {
        os << "  (empty)\n";
        return os;
    }

    for (int i = 0; i < ol.size(); ++i) {
        os << "  [" << i << "] ";
        Order* o = ol.getOrder(i);
        if (o) {
            os << *o;
        } else {
            os << "(null order)";
        }
        os << "\n";
    }

    return os;
};
