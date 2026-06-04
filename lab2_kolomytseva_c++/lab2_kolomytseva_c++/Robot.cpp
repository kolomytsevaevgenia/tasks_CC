#include "Robot.h"
#include "Location.h"

std::string getClanName(Clan c) {
    switch (c) {
        case Clan::Red: return "Red";
        case Clan::Blue: return "Blue";
        case Clan::Green: return "Green";
    }
    return "Unknown";
}

Robot::Robot(int id, Clan c, int locId) 
    : id(id), clan(c), health(100), energy(100), locationId(locId), isActive(true) {
    moveCost = 5;
    attackCost = 10;
    idleCost = 1;
}

void Robot::setStrategy(std::unique_ptr<BehaviorStrategy> strat) {
    strategy = std::move(strat);
}

void Robot::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
}

void Robot::consumeEnergy(int amount) {
    energy -= amount;
    if (energy <= 0) {
        energy = 0;
        isActive = false;
    }
}

void Robot::recharge(int amount) {
    energy += amount;
}

void Robot::moveTo(int newLocId) {
    if (energy >= moveCost) {
        locationId = newLocId;
        consumeEnergy(moveCost);
    }
}

void Robot::act(Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap) {
    if (!isActive) return;
    
    consumeEnergy(idleCost);
    if (!isActive) return;

    if (strategy) {
        strategy->execute(this, currentLocation, worldMap);
    }
}