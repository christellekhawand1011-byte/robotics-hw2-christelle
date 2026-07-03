#pragma once
#include <string>
#include <ostream>

class Task;

/**
 * @brief Abstract base class representing a general robot.
 *
 * Robot stores the common information shared by all robot types:
 * id, name, battery level, and status.
 *
 * This class is abstract because work() and type() are pure virtual.
 */
class Robot {
public:
    /**
     * @brief Constructs a Robot.
     *
     * @param id Unique robot identifier.
     * @param name Robot display name.
     * @param battery Initial battery percentage. It is clamped to [0, 100].
     */
    Robot(const std::string& id, const std::string& name, int battery);

    /**
     * @brief Virtual destructor for safe deletion through base-class pointers.
     */
    virtual ~Robot() = default;

    /**
     * @brief Makes the robot perform its work.
     *
     * Each derived class implements this differently.
     *
     * @throws std::runtime_error if the battery is empty.
     */
    virtual void work() = 0;

    /**
     * @brief Charges the robot battery back to 100%.
     *
     * Also sets the robot status to "charging".
     */
    virtual void charge();

    /**
     * @brief Returns the robot type.
     *
     * @return A string such as "MobileRobot", "CleaningRobot", or "CookingRobot".
     */
    virtual std::string type() const = 0;

    /**
     * @brief Returns the robot id.
     * @return Robot id.
     */
    std::string id() const;

    /**
     * @brief Returns the robot name.
     * @return Robot name.
     */
    std::string name() const;

    /**
     * @brief Returns the current battery level.
     * @return Battery percentage in [0, 100].
     */
    int battery() const;

    /**
     * @brief Returns the current robot status.
     * @return Status string: "idle", "working", or "charging".
     */
    std::string status() const;

    /**
     * @brief Compares two robots by id.
     *
     * @param other The robot to compare with.
     * @return true if both robots have the same id, false otherwise.
     */
    bool operator==(const Robot& other) const;

    /**
     * @brief Combines two robot names.
     *
     * @param other The other robot.
     * @return A string like "R2D2 + C3PO".
     */
    std::string operator+(const Robot& other) const;

    /**
     * @brief Prints a robot in a readable format.
     *
     * operator<< must be a free function because the left side is std::ostream,
     * for example: std::cout << robot means operator<<(std::cout, robot).
     * If it were a Robot member function, the syntax would be robot << std::cout,
     * which is not what we want.
     *
     * It is declared as friend so it can access the robot's protected attributes
     * directly when printing.
     *
     * @param os Output stream.
     * @param r Robot to print.
     * @return The same output stream, allowing chained printing.
     */
    friend std::ostream& operator<<(std::ostream& os, const Robot& r);

protected:
    std::string id_;       ///< Unique robot identifier.
    std::string name_;     ///< Robot display name.
    int battery_;          ///< Battery level, always clamped to [0, 100].
    std::string status_;   ///< Current status: "idle", "working", or "charging".
};