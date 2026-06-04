#include "World.h"
#include "WheeledRobot.h"
#include "LeggedRobot.h"
#include <iostream>

World::World() : nextRobotId(1), maxTurns(50) {}

void World::createLocation(int id) {
    if (locations.find(id) == locations.end()) {
        locations[id] = std::make_shared<Location>(id);
    }
}

void World::connectLocations(int id1, int id2) {
    if (locations.count(id1) && locations.count(id2)) {
        locations[id1]->addNeighbor(id2);
        locations[id2]->addNeighbor(id1);
    }
}

void World::spawnRobot(int locId, Clan clan, RobotType type, std::unique_ptr<BehaviorStrategy> strategy) {
    if (locations.find(locId) == locations.end()) return;

    std::shared_ptr<Robot> newRobot;
    if (type == RobotType::Wheeled) {
        newRobot = std::make_shared<WheeledRobot>(nextRobotId++, clan, locId);
    } else {
        newRobot = std::make_shared<LeggedRobot>(nextRobotId++, clan, locId);
    }

    newRobot->setStrategy(std::move(strategy));
    locations[locId]->addRobot(newRobot);
}

bool World::checkWinCondition() {
    std::set<Clan> activeClans;
    for (const auto& pair : locations) {
        for (const auto& robot : pair.second->getRobots()) {
            if (robot->getIsActive()) {
                activeClans.insert(robot->getClan());
            }
        }
    }

    std::cout << "Active Clans: ";
    for (auto c : activeClans) std::cout << getClanName(c) << " ";
    std::cout << std::endl;

    return activeClans.size() <= 1;
}

void World::runSimulation() {
    std::cout << "\n--- Starting Simulation ---\n" << std::endl;
    
    for (int turn = 1; turn <= maxTurns; ++turn) {
        std::cout << "=== Turn " << turn << " ===" << std::endl;
        
        for (auto& pair : locations) {
            pair.second->updateRobots(locations);
        }

        if (checkWinCondition()) {
            std::cout << "\n*** GAME OVER ***\n" << std::endl;
            break;
        }
        std::cout << std::endl;
    }
    std::cout << "Simulation finished." << std::endl;
}