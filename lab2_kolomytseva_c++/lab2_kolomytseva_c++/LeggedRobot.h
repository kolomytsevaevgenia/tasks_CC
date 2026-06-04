#ifndef LEGGED_ROBOT_H
#define LEGGED_ROBOT_H

#include "Robot.h"

class LeggedRobot : public Robot {
public:
    LeggedRobot(int id, Clan c, int locId);
    RobotType getType() const override;
    std::string getClassName() const override;
};

#endif // LEGGED_ROBOT_H