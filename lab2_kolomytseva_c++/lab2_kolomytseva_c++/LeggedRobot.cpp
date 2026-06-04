#include "LeggedRobot.h"

LeggedRobot::LeggedRobot(int id, Clan c, int locId) : Robot(id, c, locId) {
    damage = 20;
    moveCost = 7; 
    attackCost = 15;
}

RobotType LeggedRobot::getType() const {
    return RobotType::Legged;
}

std::string LeggedRobot::getClassName() const {
    return "LeggedBot";
}