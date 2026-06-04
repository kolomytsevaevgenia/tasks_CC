#ifndef BEHAVIOR_STRATEGY_H
#define BEHAVIOR_STRATEGY_H

#include <string>
#include <map>
#include <memory>

class Robot;
class Location;

class BehaviorStrategy {
public:
    virtual ~BehaviorStrategy() = default;
    virtual void execute(Robot* robot, Location* currentLocation, 
                        const std::map<int, std::shared_ptr<Location>>& worldMap) = 0;
    virtual std::string getName() const = 0;
};

#endif // BEHAVIOR_STRATEGY_H