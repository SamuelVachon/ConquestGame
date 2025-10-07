// Header Guards
#ifndef ORDERS_H
#define ORDERS_H
 
#include <string>
#include <iostream>
using namespace std;
#include <vector>
class Player;   // Forward declaration of Player class
class Territory;   // Forward declaration of Territory class

class Order{

    private:
    
    // Attributes
    string* description;
    string* effect;
    Player* issuer;
    Territory* target;
    
    public:

    // Getters
    string getDescription() const;
    string getEffect() const;
    Player* getIssuer() const;
    Territory* getTarget() const;
    
    // Setters
    void setDescription(const string& description);
    void setEffect(const string& effect);
    void setIssuer(Player* issuer);
    void setTarget(Territory* target);

    // Default Constructor
    Order();

    // Parameterized Constructor
    Order(const string& description, const string& effect, Player* issuer=nullptr, Territory* target=nullptr);

    //Copy Constructor
    Order(const Order& other);

    // Destructor
    virtual ~Order();

    // Assignment Operator
    Order& operator=(const Order& other);

    virtual bool validate() const=0;
    virtual void execute()=0;

    // Operator
    friend ostream& operator <<(ostream& os, const Order& order);

    
};

// subclass
class DeployOrder:  public Order{
    
    // Attributes
    private:
    int* numArmies;

    public:

    // getter
    int getNumArmies() const;

    //Setter
    void setNumArmies(int numArmies);

    // Default Constructor
    DeployOrder();

    // Paramaterized Constructor
    DeployOrder(Player* issuer, Territory* target, int numArmies);

    // Copy Constructor
    DeployOrder(const DeployOrder& other);

    // Assignemnt Operator
    DeployOrder& operator=(const DeployOrder& other);

    // Destructor
    ~DeployOrder() override;
    
    //  Core Methods 
    bool validate() override;
    void execute() override;

    // operator

    friend ostream& operator<<(ostream& os, const DeployOrder& order);


};

  // CLASS: AdvanceOrder
  // Represents an Advance order: moves armies from a source territory to a target
  //territory

class AdvanceOrder: public Order{

    // Attributes
    private:
    Territory* source;
    int* numArmies;

    public:

    // Getters
    Territory* getSource() const;
    int getNumArmies() const;

    // Setters
    void setSource(Territory* source);
    void setNumArmies(int numArmies);

    // Default Constructor
    AdvanceOrder();

    // Parameterized Constructor
    AdvanceOrder(Player* issuer,Territory* source, Territory* target, const int& numArmies);

    // Copy Constructor
    AdvanceOrder(const AdvanceOrder& other);

    // Assignment Operator
    AdvanceOrder& operator=(const AdvanceOrder& other);

    // Destructor
    ~AdvanceOrder() override;

    bool validate() override;
    void execute() override;

    // operator

    friend ostream& operator<<(ostream& os, const AdvanceOrder& other);


};

// CLASS: BombOrder

// Represents a Bomb order: reduces the number of armies in an enemy territory by half

class BombOrder: public Order{

    public:
    
    // Default Constructor
    BombOrder();

    // Paramaterized Constructor
    BombOrder(Player* issuer, Territory* target);

    // Copy
    BombOrder(const BombOrder& other);

    // Assignment Operator
    BombOrder& operator=(const BombOrder& other);

    // Destructor
    ~BombOrder() override;

    //  Core Methods 
    bool validate() override;
    void execute() override;


    friend ostream& operator<<(ostream& os, const BombOrder& order);

};

// CLASS: BlockadeOrder
// Represents a Blockade order: triples the armies on a territory and transfers
// ownership to a neutral player.

class BlockadeOrder : public Order {
public:
    //  Constructors 
    BlockadeOrder();
    BlockadeOrder(Player* issuer, Territory* target);
    BlockadeOrder(const BlockadeOrder& other);
    BlockadeOrder& operator=(const BlockadeOrder& other);
    ~BlockadeOrder() override;

    // Overridden methods
    bool validate() override;   
    void execute() override;    

    //  Stream insertion operator
    friend ostream& operator<<(ostream& os, const BlockadeOrder& order);
};

// CLASS: AirliftOrder
class AirliftOrder : public Order {
private:
    Territory* source;  // origin territory
    int* numArmies;     // armies to move

public:
    //  Constructors 
    AirliftOrder();
    AirliftOrder(Player* issuer, Territory* source, Territory* target, int numArmies);
    AirliftOrder(const AirliftOrder& other);
    AirliftOrder& operator=(const AirliftOrder& other);
    ~AirliftOrder() override;

    //  Getters & Setters
    Territory* getSource() const;
    int getNumArmies() const;
    void setSource(Territory* source);
    void setNumArmies(int numArmies);

    //  Overridden methods 
    bool validate() override;   
    void execute() override;  

    // Stream insertion operator
    friend ostream& operator<<(ostream& os, const AirliftOrder& order);
};

// CLASS: NegotiateOrder

class NegotiateOrder : public Order {
private:
    Player* otherPlayer;   // the second player in the negotiation

public:
    //  Constructors 
    NegotiateOrder();
    NegotiateOrder(Player* issuer, Player* otherPlayer);
    NegotiateOrder(const NegotiateOrder& other);
    NegotiateOrder& operator=(const NegotiateOrder& other);
    ~NegotiateOrder() override;

    // Getters & Setters 
    Player* getOtherPlayer() const;
    void setOtherPlayer(Player* otherPlayer);

    //  Overridden methods 
    bool validate() override;   
    void execute() override;    

    //  Stream insertion operator 
    friend ostream& operator<<(ostream& os, const NegotiateOrder& order);
};

// CLASS: OrdersList

class OrdersList{

    private:

    vector<Order*>* orders; // To hold Order Objects

    public:

    //Constructors
    OrdersList();
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    ~OrdersList();

    
    //  List Operations 

    Order* getOrder(int index) const;   // Returns an order by index
    int size() const;                   // Returns number of orders
    void removeOrder(int index);        // Deletes an order at index
    void addOrder(Order* order);        // Adds a new order to the list
    void moveOrder(int fromIndex, int toIndex);  // Moves an order between positions

    // Stream operator

    friend ostream& operator<<(ostream& os, const OrdersList& ol);


};

#endif