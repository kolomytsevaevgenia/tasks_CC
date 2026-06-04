#include <iostream>
#include <ctime>
#include "World.h"
#include "Strategies.h"

int main() {
    srand(static_cast<unsigned int>(time(0)));

    World gameWorld;

    // 1. Создание мира
    gameWorld.createLocation(1);
    gameWorld.createLocation(2);
    gameWorld.createLocation(3);
    gameWorld.createLocation(4);

    // 2. Связывание локаций
    gameWorld.connectLocations(1, 2);
    gameWorld.connectLocations(2, 3);
    gameWorld.connectLocations(3, 4);
    gameWorld.connectLocations(4, 1); 

    // 3. Создание роботов
    gameWorld.spawnRobot(1, Clan::Red, RobotType::Wheeled, std::make_unique<HunterStrategy>());
    gameWorld.spawnRobot(1, Clan::Red, RobotType::Legged, std::make_unique<HunterStrategy>());
    
    gameWorld.spawnRobot(3, Clan::Blue, RobotType::Wheeled, std::make_unique<ExplorerStrategy>());
    gameWorld.spawnRobot(3, Clan::Blue, RobotType::Legged, std::make_unique<DefenderStrategy>());

    gameWorld.spawnRobot(2, Clan::Green, RobotType::Wheeled, std::make_unique<DefenderStrategy>());
    gameWorld.spawnRobot(4, Clan::Green, RobotType::Legged, std::make_unique<HunterStrategy>());

    std::cout << "World initialized with 3 Clans." << std::endl;

    gameWorld.runSimulation();

    return 0;
}