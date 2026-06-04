#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <memory>
#include <set>
#include "Location.h"
#include "Robot.h"

class World {
private:
    std::map<int, std::shared_ptr<Location>> locations;
    int nextRobotId;
    int maxTurns;

public:
    World();

    void createLocation(int id);
    void connectLocations(int id1, int id2);
    void spawnRobot(int locId, Clan clan, RobotType type, std::unique_ptr<BehaviorStrategy> strategy);
    bool checkWinCondition();
    void runSimulation();
};

#endif // WORLD_H