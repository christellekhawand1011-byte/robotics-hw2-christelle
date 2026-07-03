#include "cleaning_robot.hpp"
#include <iostream>

CleaningRobot::CleaningRobot(const std::string& id, const std::string& name,
                             int battery, double speed, int brush_power)
    : MobileRobot(id, name, battery, speed), brush_power_(brush_power) {}

void CleaningRobot::work() {
    if (battery_ == 0) {
        throw std::runtime_error(name_ + " cannot work: battery is empty");
    }
    battery_ -= 25;
    status_ = "working";

    std::cout << name_ << " is cleaning with brush power " << brush_power_ 
        << " while moving at speed " << speed_ << ".\n";
}

std::string CleaningRobot::type() const {
    return "CleaningRobot";
}