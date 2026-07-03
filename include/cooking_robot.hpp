#pragma once
#include "robot.hpp"


/**
 * @brief Robot that can cook.
 *
 * CookingRobot inherits from Robot and provides its own implementation
 * of work() and type().
 */
class CookingRobot : public Robot {
public:
    /**
     * @brief Constructs a CookingRobot given a cooking method.
     *
     * @param id Unique robot identifier.
     * @param name Robot display name.
     * @param battery Initial battery percentage, clamped by Robot to [0, 100].
     * @param cooking_method The cooking method the robot will use.
     */
    CookingRobot(const std::string& id, const std::string& name,
                 int battery, const std::string& cooking_method);

    /**
     * @brief Makes the cooking robot cook and consumes 15% battery.
     *
     * Also sets the robot status to "working".
     *
     * @throws std::runtime_error if the battery is empty.
     */
    void work() override;

    /**
     * @brief Returns the type of this robot.
     *
     * @return "CookingRobot".
     */
    std::string type() const override;

private:
    std::string cooking_method_; ///< The cooking method the robot will use.
};