#pragma once
#include "mobile_robot.hpp"
#include <thread>
#include <atomic>


/**
 * @brief A Mobile Robot specialized for cleaning tasks.
 *
 * CleaningRobot inherits from MobileRobot and provides its own implementation
 * of work() and type().
 */
class CleaningRobot : public MobileRobot {
public:
    /**
     * @brief Constructs a CleaningRobot.
     *
     * @param id Unique robot identifier.
     * @param name Robot display name.
     * @param battery Initial battery percentage, clamped by Robot to [0, 100].
     * @param speed Movement speed of the robot.
     * @param brush_power Power of the cleaning brush.
     */
    CleaningRobot(const std::string& id, const std::string& name,
                  int battery, double speed, int brush_power);

    /**
     * @brief Makes the mobile robot move and clean, consuming 25% battery.
     *
     * Also sets the robot status to "working".
     *
     * @throws std::runtime_error if the battery is empty.
     */
    void work() override;

    /**
     * @brief Returns the type of this robot.
     *
     * @return "CleaningRobot".
     */
    std::string type() const override;

private:
    int brush_power_;           ///< Power of the cleaning brush.
};