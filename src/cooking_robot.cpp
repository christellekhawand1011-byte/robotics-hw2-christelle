#include "cooking_robot.hpp"
#include <iostream>
#include <algorithm>


CookingRobot::CookingRobot(const std::string& id, const std::string& name,
                           int battery, const std::string& cooking_method)
    : Robot(id, name, battery), cooking_method_(cooking_method) {}

void CookingRobot::work() {
    if (battery_ < 15) {
        throw std::runtime_error(name_ + "cannot work: battery is empty.");
    }

    battery_ = std::max(0, battery_ - 15);
    status_ = "working";
    std::cout << name_ << " is cooking using the " << cooking_method_ << " method." << std::endl;
}

std::string CookingRobot::type() const {
    return "CookingRobot";
}