#include "fleet.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>

void Fleet::add(std::shared_ptr<Robot> robot) {
    if (!robot) {
        throw std::invalid_argument("Cannot add null robot to fleet.");
    }
    if (robots_.count(robot->id()) > 0) {
        throw std::runtime_error("Robot with id " + robot->id() + " already exists in fleet.");
    }
    robots_[robot->id()] = robot;
}

void Fleet::remove(const std::string& id) {
    if (robots_.erase(id) == 0) {
        throw std::runtime_error("Robot with id " + id + " does not exist in fleet.");
    }
}

std::shared_ptr<Robot> Fleet::find(const std::string& id) const {
    const auto it = robots_.find(id);
    if (it == robots_.end()) {
        throw std::runtime_error("Robot with id " + id + " does not exist in fleet.");
    }
    return it->second;
}

std::shared_ptr<Robot> Fleet::find_first_below_battery(int threshold) const {
    const auto it = std::find_if(
        robots_.begin(), robots_.end(),
        [threshold](const auto& item) 
        {return item.second->battery() < threshold;}
    );

    if (it == robots_.end()) {
        std::cout << "No robot found with battery below " << threshold << "%.\n";
        return nullptr;
    }

    std::cout << "Found robot with battery below " << threshold << "%: "
              << *(it->second) << "\n";
    return it->second;
}

void Fleet::assign_task(const std::string& robot_id, const Task& t) {
    auto robot = find(robot_id);
    Task assigned_task = t;
    assigned_task.assigned_to = robot_id;

    tasks_.push(assigned_task);
    robot->mark_working();
}

void Fleet::show_tasks() const {
    if (tasks_.empty()) {
        std::cout << "No tasks in the queue.\n";
        return;
    }

    std::priority_queue<Task> copy = tasks_;

    while (!copy.empty()) {
        std::cout << copy.top() << "\n";
        copy.pop();
    }
}

void Fleet::work_all() const {
    for (const auto& item : robots_) {
        item.second->work();
    }
}

void Fleet::charge_all() const {
    for (const auto& item : robots_) {
        item.second->charge();
    }
}

std::size_t Fleet::size() const {
    return robots_.size();
}

bool Fleet::empty() const {
    return robots_.empty();
}

Fleet& Fleet::operator+=(std::shared_ptr<Robot> robot) {
    add(robot);
    return *this;
}

Fleet& Fleet::operator-=(const std::string& id) {
    remove(id);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Fleet& f) {
    if (f.robots_.empty()) {
        os << "Fleet is empty.\n";
        return os;
    }

    os << "Fleet contains " << f.robots_.size() << " robot(s):\n";

    for (const auto& item : f.robots_) {
        os << "  " << *(item.second) << "\n";
    }

    return os;
}