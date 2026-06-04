#ifndef LOCATION_H
#define LOCATION_H

#include <vector>
#include <memory>
#include <map>
#include "Types.h"  // <-- Добавь эту строку!

class Robot;  // Forward declaration

class Location {
private:
    int id;
    std::vector<int> neighbors; 
    std::vector<std::shared_ptr<Robot>> robots;
    int batteries;

public:
    Location(int id);

    int getId() const { return id; }

    void addNeighbor(int locId);
    const std::vector<int>& getNeighbors() const { return neighbors; }

    void addRobot(std::shared_ptr<Robot> robot);
    void cleanup();

    bool hasResources() const { return batteries > 0; }
    int takeResource();

    std::vector<Robot*> getEnemies(Clan myClan);  // <-- Теперь Clan известен
    std::vector<Robot*> getAllies(Clan myClan);   // <-- Теперь Clan известен

    const std::vector<std::shared_ptr<Robot>>& getRobots() const {
        return robots;
    }

    void updateRobots(const std::map<int, std::shared_ptr<Location>>& worldMap);
    
    void printStatus() const;
};

#endif // LOCATION_H