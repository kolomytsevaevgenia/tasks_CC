#include "Strategies.h"
#include "Robot.h"
#include "Location.h"
#include <iostream>

void ExplorerStrategy::execute(Robot* robot, Location* currentLocation, 
                               const std::map<int, std::shared_ptr<Location>>& worldMap) {
    if (currentLocation->hasResources()) {
        int gained = currentLocation->takeResource();
        robot->recharge(gained);
        return; 
    }

    auto neighbors = currentLocation->getNeighbors();
    if (!neighbors.empty()) {
        int randomIdx = rand() % neighbors.size();
        robot->moveTo(neighbors[randomIdx]);
    }
}

void HunterStrategy::execute(Robot* robot, Location* currentLocation, 
                             const std::map<int, std::shared_ptr<Location>>& worldMap) {
    auto enemies = currentLocation->getEnemies(robot->getClan());

    if (!enemies.empty()) {
        if (robot->getEnergy() >= robot->getAttackCost()) {
            int targetIdx = rand() % enemies.size();
            enemies[targetIdx]->takeDamage(robot->getDamage());
            robot->consumeEnergy(robot->getAttackCost());
            std::cout << "  [Attack] Robot " << robot->getId() << " hit enemy!" << std::endl;
        }
    } else {
        ExplorerStrategy exp;
        exp.execute(robot, currentLocation, worldMap);
    }
}

void DefenderStrategy::execute(Robot* robot, Location* currentLocation, 
                               const std::map<int, std::shared_ptr<Location>>& worldMap) {
    auto allies = currentLocation->getAllies(robot->getClan());
    
    if (allies.size() > 1 || currentLocation->hasResources()) {
        if (currentLocation->hasResources()) {
            int gained = currentLocation->takeResource();
            robot->recharge(gained);
        }
        auto enemies = currentLocation->getEnemies(robot->getClan());
        if (!enemies.empty() && robot->getEnergy() >= robot->getAttackCost()) {
             int targetIdx = rand() % enemies.size();
             enemies[targetIdx]->takeDamage(robot->getDamage());
             robot->consumeEnergy(robot->getAttackCost());
        }
    } else {
        ExplorerStrategy exp;
        exp.execute(robot, currentLocation, worldMap);
    }
}