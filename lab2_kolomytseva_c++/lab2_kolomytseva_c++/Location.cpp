#include "Location.h"
#include "Robot.h"
#include <algorithm>
#include <iostream>

Location::Location(int id) : id(id), batteries(0) {
    batteries = rand() % 5; 
}

void Location::addNeighbor(int locId) {
    if (std::find(neighbors.begin(), neighbors.end(), locId) == neighbors.end()) {
        neighbors.push_back(locId);
    }
}

void Location::addRobot(std::shared_ptr<Robot> robot) {
    robots.push_back(robot);
}

void Location::cleanup() {
    robots.erase(
        std::remove_if(robots.begin(), robots.end(), 
            [](const std::shared_ptr<Robot>& r) { return !r->getIsActive(); }),
        robots.end()
    );
}

int Location::takeResource() {
    if (batteries > 0) {
        batteries--;
        return 50; 
    }
    return 0;
}

std::vector<Robot*> Location::getEnemies(Clan myClan) {
    std::vector<Robot*> enemies;
    for (auto& r : robots) {
        if (r->getIsActive() && r->getClan() != myClan) {
            enemies.push_back(r.get());
        }
    }
    return enemies;
}

std::vector<Robot*> Location::getAllies(Clan myClan) {
    std::vector<Robot*> allies;
    for (auto& r : robots) {
        if (r->getIsActive() && r->getClan() == myClan) {
            allies.push_back(r.get());
        }
    }
    return allies;
}

void Location::updateRobots(const std::map<int, std::shared_ptr<Location>>& worldMap) {
    for (auto& r : robots) {
        if (r->getIsActive()) {
            r->act(this, worldMap);
        }
    }
    cleanup();
}

void Location::printStatus() const {
    std::cout << "  Loc[" << id << "] Bat:" << batteries << " AliveRobots:" << robots.size() << std::endl;
}