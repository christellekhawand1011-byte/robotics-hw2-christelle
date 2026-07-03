#pragma once
#include <string>
#include <ostream>

/**
 * @brief Describes a task that can be assigned to a robot.
 *
 * A Task has a name, a priority from 1 to 5, and the id of the robot it is assigned to.
 */
struct Task {
    std::string name;         ///< Task name.
    int         priority;     ///< Task priority: 1 is low, 5 is high.
    std::string assigned_to;  ///< Id of the robot assigned to this task.

    /**
     * @brief Compares two tasks by priority.
     *
     * This is used by std::priority_queue so that tasks with higher
     * priority come first.
     *
     * @param other The task to compare with.
     * @return true if this task has lower priority than the other task.
     */
    bool operator<(const Task& other) const;

    /**
     * @brief Prints a task in a readable format.
     *
     * @param os Output stream.
     * @param t Task to print.
     * @return The same output stream, allowing chained printing.
     */
    friend std::ostream& operator<<(std::ostream& os, const Task& t);
};