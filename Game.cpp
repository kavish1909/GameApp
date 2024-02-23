#include "Game.h"
#include <algorithm>

// -------------------- Object class ----------------------

Object::Object(string name, int value) : name_(name), value_(value), owner_(nullptr)
{
}

Object::~Object() {}

string Object::getName() const
{
        return name_;
}

int Object::getValue() const
{
        return value_;
}

string Object::print() const
{
        return "";
}

void Object::use()
{
}

std::ostream &operator<<(std::ostream &os, const Object &o)
{
        os << o.print();
        return os;
}

// ------------------- Food class ----------------------

Food::Food(string name, int value) : Object(name, value)       // call constructor of base class
{
       if(value<0)
        throw std::invalid_argument("Negative value is passed for Food.");
}

Food::~Food() {}

string Food::print() const
{

        return "Food, name: " + Object::getName() + ", value: " + std::to_string(Object::getValue());
}

void Food::use()
{
        if (owner_->getStamina() + value_ > 100)
                owner_->setStamina(100);                   // call setStamina function in player class and set max cap of stamina to 100
        else
                owner_->setStamina(owner_->getStamina() + value_);
        owner_->foodInUse(true);
}

// ------------------- Weapon class ----------------------

Weapon::Weapon(string name, int value) : Object(name, value)
{
       if(value<0)
        throw std::invalid_argument("Negative value is passed for Weapon.");
}

Weapon::~Weapon() {}

string Weapon::print() const
{
        return "Weapon, name: " + Object::getName() + ", value: " + std::to_string(Object::getValue());
}

void Weapon::use()
{
        owner_->setWeaponInUse(name_, value_);           // pass value to store attacking strength and name for wepaon in use
}

// ------------------- Armour class ----------------------

Armour::Armour(string name, int value) : Object(name, value)
{
      if(value<0)
        throw std::invalid_argument("Negative value is passed for Armour.");
}

Armour::~Armour() {}

string Armour::print() const
{
        return "Armour, name: " + Object::getName() + ", value: " + std::to_string(Object::getValue());
}

void Armour::use()
{
        owner_->setArmourInUse(name_, value_);           // pass value to store defending strength and name for armour in use
}

// ------------------- Player class ----------------------

Player::Player(string name) : playerName_(name), health_(100), stamina_(100)
{
}

Player::~Player() {}

string Player::getName() const
{
        return playerName_;
}

int Player::getHealth() const
{
        return health_;
}

int Player::getStamina() const
{
        return stamina_;
}

void Player::setStamina(int value)
{
        stamina_ = value;
}

void Player::pickup(std::unique_ptr<Object> o)
{
        if (getHealth() == 0)
                throw std::logic_error("Dead player");
        o->owner_ = this;                                                      // Point object's owner to current player
        inventory_.push_back(std::move(o));
}

string Player::getInventory() const
{

        string inventoryValues_ = "";
        if (inventory_.empty())                                                // Check if inventory vector has no items
                inventoryValues_ = "List of items: none";
        else
        {
                inventoryValues_ = "List of items:";
                for (int i = 0; i < inventory_.size(); i++)
                {
                        inventoryValues_ += "\n " + inventory_[i]->print();    // call print function of each object stored in vector
                }
        }
        return inventoryValues_;
}

string Player::print() const
{
        return "";
}

bool Player::use(string name)
{
        if (getHealth() == 0)
                throw std::logic_error("Dead player");

        auto it = std::find_if(inventory_.begin(), inventory_.end(), [&](const std::unique_ptr<Object> &item)
                  { return item->getName() == name; });                 // find object with given name exists in player's inventory
        if (it == inventory_.end())                                     // object with given name not found in inventory
        {
                return false;
        }

        (*it)->use();                                                  // calls the use() function of the object
        if (foodInUse_)
                inventory_.erase(it);                                  // remove food item from inventory after it is used
        return true;
}

std::ostream &operator<<(std::ostream &os, const Player &p)
{
        os << p.print();
        return os;
}

void Player::increaseHealth(int value)
{
        health_ += value;
        if (health_ > 100)
        {
                health_ = 100;
        }
}

void Player::foodInUse(bool value)
{
        foodInUse_ = value;
}

void Player::setWeaponInUse(string weaponName, int weaponValue)
{
        weaponInUseString_ = weaponName;
        weaponValue_ = weaponValue;
        weaponInUse_ = true;
}

void Player::setArmourInUse(string armourName, int armourValue)
{
        armourVector.push_back(armourName);                                    // vector to store armours
        armourValue_ += armourValue;
        armourInUse_ = true;
}

string Player::armourInUseString() const
{
        string armourInUseString_ = "";
        if (armourInUse_)
        {
                for (auto &a : armourVector)
                        armourInUseString_ += ", " + a;                       // concat all armour(s) in use
                armourInUseString_ = armourInUseString_.substr(2);
        }
        else
                armourInUseString_ = "none";
        return armourInUseString_;
}

void Player::setHealth(int value)
{
        health_ = value;
}

int Player::getArmourValue() const
{
        return armourValue_;
}

bool Player::armourInUse()
{
        return armourInUse_;
}
// ------------------- Fighter class ----------------------

Fighter::Fighter(string name) : Player(name)
{
}

Fighter::~Fighter() {}

bool Fighter::attack(Player &other)
{

        if (getHealth() == 0)
                throw std::logic_error("Dead Attacker.");
        else if (other.getHealth() == 0)
                throw std::logic_error("Stop! " + other.getName() + " is already dead.");

        if (getStamina() < 10)
                return false;

        stamina_ = stamina_ - 10;
        int attack_strength = 10;
        if (weaponInUse_)                                                  // if weapon is used increase attacking strength by value of weapon
                attack_strength += weaponValue_;
        int defend_strength = 0;
        if (other.armourInUse())                                          // if armour is used assign it to defending strength
                defend_strength = other.getArmourValue();
        if (attack_strength > defend_strength)
        {
                other.setHealth(other.getHealth() - (attack_strength - defend_strength));
                if (other.getHealth() <= 0)
                        other.setHealth(0);
                return true;
        }
        else
        {
                return false;
        }
}

string Fighter::print() const
{

        string fighterDetails = "Name: " + getName() + "\nType: Fighter\nHealth: " + std::to_string(getHealth()) + "\nStamina: " + std::to_string(getStamina())
                                + "\n" + getInventory() + "\nWeapon in use: " + weaponInUseString_ + "\nArmour in use: " + armourInUseString();
        return fighterDetails;
}

// ------------------- Healer class ----------------------

Healer::Healer(string name) : Player(name)
{
}

Healer::~Healer() {}

bool Healer::heal(Player &other)
{
        if (other.getHealth() == 0 || getHealth() == 0)                   // check if healer or player being healed is already dead
                throw std::logic_error("Dead Player");
        if (stamina_ < 10)
                return false;
        else
                stamina_ = stamina_ - 10;
        if (other.getHealth() >= 100)
                return false;
        else
                other.increaseHealth(20);
        return true;
}

string Healer::print() const
{
        string healerDetails = "Name: " + getName() + "\nType: Healer\nHealth: " + std::to_string(getHealth()) + "\nStamina: " + std::to_string(getStamina())
                               + "\n" + getInventory() + "\nArmour in use: " + armourInUseString();
        return healerDetails;
}