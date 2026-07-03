#pragma once
#include "robot.hpp"
#include "task.hpp"
#include <memory>
#include <string>
#include <ostream>
#include <queue>
#include <unordered_map>

/**
 * @brief Manages a collection of robots and a queue of assigned tasks.
 *
 * Fleet stores robots by id and allows adding, removing, finding,
 * assigning tasks, working all robots, and charging all robots.
 */
class Fleet {
public:
    /**
     * @brief Adds a robot to the fleet.
     * @param robot Shared pointer to the robot to add.
     * @throws std::invalid_argument if robot is null.
     * @throws std::runtime_error if a robot with the same id already exists.
     */
    void add(std::shared_ptr<Robot> robot);

    /**
     * @brief Removes a robot from the fleet by id.
     * @param id Robot id to remove.
     * @throws std::runtime_error if the robot id does not exist.
     */
    void remove(const std::string& id);

    /**
     * @brief Finds a robot by id.
     * @param id Robot id to search for.
     * @return Shared pointer to the found robot.
     * @throws std::runtime_error if the robot id does not exist.
     */
    std::shared_ptr<Robot> find(const std::string& id) const;

    /**
    * @brief Finds the first robot with battery below a given threshold.
    *
    * Uses std::find_if with a lambda to search the robot collection.
    *
    * @param threshold Battery percentage threshold.
    * @return Shared pointer to the first matching robot, or nullptr if none is found.
    */
    std::shared_ptr<Robot> find_first_below_battery(int threshold) const;

    /**
     * @brief Assigns a task to a robot.
     *
     * The task is copied, its assigned_to field is set to robot_id,
     * then it is stored in the priority queue.
     *
     * @param robot_id Id of the robot receiving the task.
     * @param t Task to assign.
     * @throws std::runtime_error if the robot id does not exist.
     */
    void assign_task(const std::string& robot_id, const Task& t);

    /**
     * @brief Prints the task queue in priority order.
     */
    void show_tasks() const;

    /**
     * @brief Calls work() on every robot in the fleet.
     */
    void work_all() const;

    /**
     * @brief Calls charge() on every robot in the fleet.
     */
    void charge_all() const;

    /**
     * @brief Returns the number of robots in the fleet.
     * @return Fleet size.
     */
    std::size_t size() const;

    /**
     * @brief Checks if the fleet is empty.
     * @return true if the fleet has no robots, false otherwise.
     */
    bool empty() const;

    /**
     * @brief Adds a robot using +=.
     * @param robot Shared pointer to the robot to add.
     * @return Reference to this fleet.
     */
    Fleet& operator+=(std::shared_ptr<Robot> robot);

    /**
     * @brief Removes a robot using -=.
     * @param id Robot id to remove.
     * @return Reference to this fleet.
     */
    Fleet& operator-=(const std::string& id);

    /**
     * @brief Prints all robots in the fleet.
     * @param os Output stream.
     * @param f Fleet to print.
     * @return The same output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Fleet& f);

private:
    // shared_ptr is used because robots can be owned by the fleet while still
    // being referenced elsewhere in the program, for example in main or menus.
    // With unique_ptr, ownership would move into the fleet and external shared
    // access would not be allowed.
    //
    // unordered_map gives fast lookup by robot id, with average O(1) cost.
    std::unordered_map<std::string, std::shared_ptr<Robot>> robots_;

    // priority_queue is built on a vector heap by default.
    // It is the right fit because tasks must always come out highest priority first.
    std::priority_queue<Task> tasks_;
};