#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <string>
#include <vector>
#include <memory>
using std::string;

class Player; // forward declaration so that Object can refer to Player

// ------------------ Object and its subclasses -------------------

class Object
{
public:
        // Default constructor, just to make this release version compilable.
        // If your implementation is correct this should be removed
        // Constructor, specifying the name and the value of an object.
        Object(string name, int value);

        // Destructor
        virtual ~Object();

        // Return the name of the object.
        string getName() const;

        // Return the value of the object.
        int getValue() const;

        // Return a string giving details of the object, as specified in
        // the assignment webpage.
        virtual string print() const = 0;//pure virtual function

private:
        // Use this object.
        // This function should not be public; it only makes sense to be
        // called from within a Player object (the player who owns it)
        virtual void use() = 0;

protected:
        // You probably want to have something like this.
        // For technical reason it may be easier to just use a raw pointer
        Player *owner_;
        string name_;
        int value_;
        // TODO: add any other protected/private member variables

        // Overloaded output stream redirection operator, printing the object to the
        // given output stream
        friend std::ostream &operator<<(std::ostream &os, const Object &o);
        friend class Player;
        // You can "friend" other classes
};

class Food : public Object
{
public:
        // Constructor
        Food(string name, int value);

        // Destructor
        virtual ~Food() override;

        // int getValue() const override;

        string print() const override;
        void use() override;
        // Add any member functions if needed

private:
        // Add any member variables if needed
};

class Weapon : public Object
{
public:
        // Constructor
        Weapon(string name, int value);

        // Destructor
        virtual ~Weapon() override;

        string print() const override;

        void use() override;
        // Add any member functions if needed

private:
        // Add any member variables if needed
};

class Armour : public Object
{
public:
        // Constructor
        Armour(string name, int value);

        // Destructor
        virtual ~Armour() override;

        string print() const override;

        void use() override;
        // Add any member functions if needed

private:
        // Add any member variables if needed
};

// ----------------- Player and its subclasses --------------------

class Player
{
public:
        // Default constructor, just to make this release version compilable.
        // If your implementation is correct this should be removed

        // Constructor, specifying the name of the player
        // Set the health and stamina both to 100
        Player(string name);

        // Destructor
        virtual ~Player();

        // Return the name of the player
        string getName() const;

        // Return the current health of the player
        int getHealth() const;

        // Return the current stamina of the player
        int getStamina() const;

        // Add the object pointed to by the unique pointer to the
        // inventory of objects this player carries.
        // As this is a unique pointer, it has to be "moved" into this
        // function using std::move().
        void pickup(std::unique_ptr<Object> obj);

        // Return a string listing all objects this player owns, in
        // the format specified in the assignment page
        string getInventory() const;

        // Return a string that contains all details of a player, as
        // specified in the assignment page
        virtual string print() const = 0;

        // Use the object with the given name. If the player does not
        // own any object of this name, return false and do nothing,
        // otherwise return true.
        bool use(string name);

        //check if food is in use.
        void foodInUse(bool value);

        //set values for attacking and set weaponInUse variable.
        void setWeaponInUse(string weaponName, int weaponValue);

        //set Health of a player.
        void setHealth(int value);

        //increase health of a player after using object
        void increaseHealth(int value);

        //set stamina after using object
        void setStamina(int value);

        //add all armour values and store armour in container
        void setArmourInUse(string armourName, int armourValue);

        //create a string for armour in use
        string armourInUseString() const;

        //get value of armours in use
        int getArmourValue() const;

        //check if armour is in use
        bool armourInUse();

protected:
        // TODO: add any protected or private member variables

        // Overloaded stream redirection operator, that prints the contents of
        // the player to the given output stream
        friend std::ostream &operator<<(std::ostream &os, const Player &p);
        string playerName_;
        int health_, stamina_;
        std::vector<std::unique_ptr<Object>> inventory_;
        bool foodInUse_ = false;
        bool weaponInUse_ = false;
        string weaponInUseString_ = "none";
        int weaponValue_ = 0;
        bool armourInUse_ = false;
        int armourValue_ = 0;
        std::vector<string> armourVector;
        // You can "friend" other classes
};

class Fighter : public Player
{
public:
        // Constructor
        Fighter(string name);

        // Destructor
        virtual ~Fighter() override;

        // Attack the specified player, following the rules in the
        // assignment page
        bool attack(Player &other);

        // Add any other functions if needed
        string print() const override;

private:
        // add any member variables if needed
};

class Healer : public Player
{
public:
        // Constructor
        Healer(string name);

        // Destructor
        virtual ~Healer() override;

        // Heal the specified player, following the rules in the
        // assignment page
        bool heal(Player &other);
        string print() const override;

private:
        // add any member variables if needed
};

#endif /* GAME_H_ */