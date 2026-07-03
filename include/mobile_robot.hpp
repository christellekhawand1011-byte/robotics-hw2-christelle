#pragma once
#include "robot.hpp"
#include <thread>
#include <atomic>


/**
 * @brief Robot that can move using a given speed.
 *
 * MobileRobot inherits from Robot and provides its own implementation
 * of work() and type().
 */
class MobileRobot : public Robot {
public:
    /**
     * @brief Constructs a MobileRobot.
     *
     * @param id Unique robot identifier.
     * @param name Robot display name.
     * @param battery Initial battery percentage, clamped by Robot to [0, 100].
     * @param speed Movement speed of the robot.
     */
    MobileRobot(const std::string& id, const std::string& name,
                int battery, double speed);

    /**
     * @brief Stops the background thread safely before destroying the robot.
     */
    ~MobileRobot() override;

    /**
     * @brief Makes the mobile robot move and consumes 20% battery.
     *
     * Also sets the robot status to "working".
     *
     * @throws std::runtime_error if the battery is empty.
     */
    void work() override;

    /**
     * @brief Returns the type of this robot.
     *
     * @return "MobileRobot".
     */
    std::string type() const override;

    /**
     * @brief Starts timed work in a background thread.
     *
     * The thread calls work() once per second for the given duration
     * and prints the robot status after each call.
     *
     * @param seconds Number of seconds to work.
     */
    void start_work(int seconds);

protected:
    double speed_;              ///< Movement speed of the robot.
    std::thread worker_;        ///< Background thread used for timed work.
    std::atomic<bool> stop_{false}; ///< Flag used to stop the worker thread safely.
};