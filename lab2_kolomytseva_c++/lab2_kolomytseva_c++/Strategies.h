#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "BehaviorStrategy.h"

class ExplorerStrategy : public BehaviorStrategy {
public:
    void execute(Robot* robot, Location* currentLocation, 
                const std::map<int, std::shared_ptr<Location>>& worldMap) override;
    std::string getName() const override { return "Explorer"; }
};

class HunterStrategy : public BehaviorStrategy {
public:
    void execute(Robot* robot, Location* currentLocation, 
                const std::map<int, std::shared_ptr<Location>>& worldMap) override;
    std::string getName() const override { return "Hunter"; }
};

class DefenderStrategy : public BehaviorStrategy {
public:
    void execute(Robot* robot, Location* currentLocation, 
                const std::map<int, std::shared_ptr<Location>>& worldMap) override;
    std::string getName() const override { return "Defender"; }
};

#endif // STRATEGIES_H