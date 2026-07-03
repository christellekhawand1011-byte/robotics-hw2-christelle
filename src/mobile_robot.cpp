#include "mobile_robot.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <stdexcept>

MobileRobot::MobileRobot(const std::string& id, const std::string& name,
                         int battery, double speed)
    : Robot(id, name, battery), speed_(speed) {}

MobileRobot::~MobileRobot() {
    stop_ = true;

    if (worker_.joinable()) {
        worker_.join();
    }
}

void MobileRobot::work() {
    if (battery_ == 0) {
        throw std::runtime_error(name_ + " cannot work: battery is empty");
    }

    status_ = "working";
    battery_ = std::max(0, battery_ - 20);

    std::cout << name_ << " is moving at speed " << speed_ << ".\n";
}

std::string MobileRobot::type() const {
    return "MobileRobot";
}

void MobileRobot::start_work(int seconds) {
    if (seconds <= 0) {
        std::cout << "Duration must be positive.\n";
        return;
    }

    if (worker_.joinable()) {
        stop_ = true;
        worker_.join();
    }

    stop_ = false;

    worker_ = std::thread([this, seconds]() {
        for (int i = 0; i < seconds && !stop_; ++i) {
            try {
                work();
                std::cout << *this << "\n";
            } catch (const std::runtime_error& e) {
                std::cout << "Error: " << e.what() << "\n";
                stop_ = true;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}