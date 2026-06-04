#include "WheeledRobot.h"

WheeledRobot::WheeledRobot(int id, Clan c, int locId) : Robot(id, c, locId) {
    damage = 10;
    moveCost = 3; 
    attackCost = 8;
}

RobotType WheeledRobot::getType() const {
    return RobotType::Wheeled;
}

std::string WheeledRobot::getClassName() const {
    return "WheeledBot";
}