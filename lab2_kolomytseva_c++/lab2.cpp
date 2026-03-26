#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <set>
#include <ctime>
#include <functional>

// ==========================================
// Константы и Перечисления
// ==========================================

enum class Clan { Red, Blue, Green };
enum class RobotType { Wheeled, Legged };

std::string getClanName(Clan c) {
    switch (c) {
        case Clan::Red: return "Red";
        case Clan::Blue: return "Blue";
        case Clan::Green: return "Green";
    }
    return "Unknown";
}

// ==========================================
// Forward Declarations
// ==========================================
class Location;
class Robot;
class BehaviorStrategy;

// ==========================================
// Стратегии поведения (Интерфейс)
// ==========================================

class BehaviorStrategy {
public:
    virtual ~BehaviorStrategy() = default;
    virtual void execute(Robot* robot, Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap) = 0;
    virtual std::string getName() const = 0;
};

// ==========================================
// Класс Робота (Базовый)
// ==========================================

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
    Robot(int id, Clan c, int locId) 
        : id(id), clan(c), health(100), energy(100), locationId(locId), isActive(true) {
        moveCost = 5;
        attackCost = 10;
        idleCost = 1;
    }

    virtual ~Robot() = default;

    void setStrategy(std::unique_ptr<BehaviorStrategy> strat) {
        strategy = std::move(strat);
    }

    // Геттеры для доступа из Location и стратегий
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

    void takeDamage(int dmg) {
        health -= dmg;
        if (health <= 0) {
            health = 0;
            isActive = false;
        }
    }

    void consumeEnergy(int amount) {
        energy -= amount;
        if (energy <= 0) {
            energy = 0;
            isActive = false;
        }
    }

    void recharge(int amount) {
        energy += amount;
    }

    void moveTo(int newLocId) {
        if (energy >= moveCost) {
            locationId = newLocId;
            consumeEnergy(moveCost);
        }
    }

    void act(Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap);
};

// ==========================================
// Классы Роботов (Наследование)
// ==========================================

class WheeledRobot : public Robot {
public:
    WheeledRobot(int id, Clan c, int locId) : Robot(id, c, locId) {
        damage = 10;
        moveCost = 3; 
        attackCost = 8;
    }
    RobotType getType() const override { return RobotType::Wheeled; }
    std::string getClassName() const override { return "WheeledBot"; }
};

class LeggedRobot : public Robot {
public:
    LeggedRobot(int id, Clan c, int locId) : Robot(id, c, locId) {
        damage = 20;
        moveCost = 7; 
        attackCost = 15;
    }
    RobotType getType() const override { return RobotType::Legged; }
    std::string getClassName() const override { return "LeggedBot"; }
};

// ==========================================
// Локация
// ==========================================

class Location {
private:
    int id;
    std::vector<int> neighbors; 
    std::vector<std::shared_ptr<Robot>> robots;
    int batteries;

public:
    Location(int id) : id(id), batteries(0) {
        batteries = rand() % 5; 
    }

    int getId() const { return id; }

    void addNeighbor(int locId) {
        if (std::find(neighbors.begin(), neighbors.end(), locId) == neighbors.end()) {
            neighbors.push_back(locId);
        }
    }

    const std::vector<int>& getNeighbors() const { return neighbors; }

    void addRobot(std::shared_ptr<Robot> robot) {
        robots.push_back(robot);
    }

    void cleanup() {
        robots.erase(
            std::remove_if(robots.begin(), robots.end(), 
                [](const std::shared_ptr<Robot>& r) { return !r->getIsActive(); }),
            robots.end()
        );
    }

    bool hasResources() const { return batteries > 0; }

    int takeResource() {
        if (batteries > 0) {
            batteries--;
            return 50; 
        }
        return 0;
    }

    std::vector<Robot*> getEnemies(Clan myClan) {
        std::vector<Robot*> enemies;
        for (auto& r : robots) {
            if (r->getIsActive() && r->getClan() != myClan) {
                enemies.push_back(r.get());
            }
        }
        return enemies;
    }

    std::vector<Robot*> getAllies(Clan myClan) {
        std::vector<Robot*> allies;
        for (auto& r : robots) {
            if (r->getIsActive() && r->getClan() == myClan) {
                allies.push_back(r.get());
            }
        }
        return allies;
    }

    const std::vector<std::shared_ptr<Robot>>& getRobots() const {
        return robots;
    }

    void updateRobots(const std::map<int, std::shared_ptr<Location>>& worldMap);
    
    void printStatus() const {
        std::cout << "  Loc[" << id << "] Bat:" << batteries << " AliveRobots:" << robots.size() << std::endl;
    }
};

// ==========================================
// Реализация стратегий
// ==========================================

class ExplorerStrategy : public BehaviorStrategy {
public:
    void execute(Robot* robot, Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap) override {
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
    std::string getName() const override { return "Explorer"; }
};

class HunterStrategy : public BehaviorStrategy {
public:
    void execute(Robot* robot, Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap) override {
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
    std::string getName() const override { return "Hunter"; }
};

class DefenderStrategy : public BehaviorStrategy {
public:
    void execute(Robot* robot, Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap) override {
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
    std::string getName() const override { return "Defender"; }
};

// ==========================================
// Реализация методов Robot и Location
// ==========================================

void Robot::act(Location* currentLocation, const std::map<int, std::shared_ptr<Location>>& worldMap) {
    if (!isActive) return;
    
    consumeEnergy(idleCost);
    if (!isActive) return;

    if (strategy) {
        strategy->execute(this, currentLocation, worldMap);
    }
}

void Location::updateRobots(const std::map<int, std::shared_ptr<Location>>& worldMap) {
    for (auto& r : robots) {
        if (r->getIsActive()) {
            r->act(this, worldMap);
        }
    }
    cleanup();
}

// ==========================================
// Мир (Контроллер)
// ==========================================

class World {
private:
    std::map<int, std::shared_ptr<Location>> locations;
    int nextRobotId;
    int maxTurns;

public:
    World() : nextRobotId(1), maxTurns(50) {}

    void createLocation(int id) {
        if (locations.find(id) == locations.end()) {
            locations[id] = std::make_shared<Location>(id);
        }
    }

    void connectLocations(int id1, int id2) {
        if (locations.count(id1) && locations.count(id2)) {
            locations[id1]->addNeighbor(id2);
            locations[id2]->addNeighbor(id1);
        }
    }

    void spawnRobot(int locId, Clan clan, RobotType type, std::unique_ptr<BehaviorStrategy> strategy) {
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

    bool checkWinCondition() {
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

    void runSimulation() {
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
};

// ==========================================
// Main
// ==========================================

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