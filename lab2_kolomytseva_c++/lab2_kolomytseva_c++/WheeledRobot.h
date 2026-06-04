#ifndef WHEELED_ROBOT_H
#define WHEELED_ROBOT_H

#include "Robot.h"

class WheeledRobot : public Robot {
public:
    WheeledRobot(int id, Clan c, int locId);
    RobotType getType() const override;
    std::string getClassName() const override;
};

#endif // WHEELED_ROBOT_H