#ifndef ROBOT_H
#define ROBOT_H

#include <memory>
#include <string>
#include <map>
#include "Types.h"  // <-- Вместо определения Clan
#include "BehaviorStrategy.h"

class Location;

class Robot {
protected:
    int id;
    Clan clan;
    int health;
    int energy;
    int damage;
    int moveCost;
    int attackCost;
    int idleCost;
    int locationId;
    std::unique_ptr<BehaviorStrategy> strategy;
    bool isActive;

public:
    Robot(int id, Clan c, int locId);
    virtual ~Robot() = default;

    void setStrategy(std::unique_ptr<BehaviorStrategy> strat);

    // Геттеры
    int getId() const { return id; }
    Clan getClan() const { return clan; }
    int getHealth() const { return health; }
    int getEnergy() const { return energy; }
    int getLocationId() const { return locationId; }
    bool getIsActive() const { return isActive; }
    int getAttackCost() const { return attackCost; }
    int getDamage() const { return damage; }
    int getMoveCost() const { return moveCost; }
    
    virtual RobotType getType() const = 0;
    virtual std::string getClassName() const = 0;

    void takeDamage(int dmg);
    void consumeEnergy(int amount);
    void recharge(int amount);
    void moveTo(int newLocId);
    void act(Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap);
};

#endif // ROBOT_H