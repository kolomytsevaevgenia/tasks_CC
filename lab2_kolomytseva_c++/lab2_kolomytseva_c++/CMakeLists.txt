cmake_minimum_required(VERSION 3.10)
project(RobotSimulation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot_sim
    main.cpp
    Robot.cpp
    WheeledRobot.cpp
    LeggedRobot.cpp
    Location.cpp
    Strategies.cpp
    World.cpp
)