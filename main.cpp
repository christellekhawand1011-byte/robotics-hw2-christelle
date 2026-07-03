#include "robot.hpp"
#include "mobile_robot.hpp"
#include "cleaning_robot.hpp"
#include "cooking_robot.hpp"
#include "fleet.hpp"

#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>

/**
 * @brief Clears invalid input from std::cin.
 */
void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Reads an integer safely from the user.
 *
 * @param prompt Message shown to the user.
 * @return Valid integer entered by the user.
 */
int read_int(const std::string& prompt) {
    int value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            clear_input();
            return value;
        }

        std::cout << "Invalid input. Please enter a number.\n";
        clear_input();
    }
}

/**
 * @brief Reads a positive integer safely from the user.
 *
 * @param prompt Message shown to the user.
 * @return Positive integer entered by the user.
 */
int read_positive_int(const std::string& prompt) {
    while (true) {
        int value = read_int(prompt);

        if (value > 0) {
            return value;
        }

        std::cout << "Value must be positive.\n";
    }
}

/**
 * @brief Reads a double safely from the user.
 *
 * @param prompt Message shown to the user.
 * @return Valid double entered by the user.
 */
double read_double(const std::string& prompt) {
    double value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            clear_input();
            return value;
        }

        std::cout << "Invalid input. Please enter a number.\n";
        clear_input();
    }
}

/**
 * @brief Reads a non-empty line from the user.
 *
 * @param prompt Message shown to the user.
 * @return Non-empty string entered by the user.
 */
std::string read_non_empty_string(const std::string& prompt) {
    std::string value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);

        if (!value.empty()) {
            return value;
        }

        std::cout << "Input cannot be empty.\n";
    }
}

/**
 * @brief Prints the main menu.
 */
void print_menu() {
    std::cout << "\n=== Robot Fleet Manager ===\n";
    std::cout << "1. Add robot\n";
    std::cout << "2. Remove robot\n";
    std::cout << "3. Show all robots\n";
    std::cout << "4. Work single robot\n";
    std::cout << "5. Work all\n";
    std::cout << "6. Charge single robot\n";
    std::cout << "7. Charge all\n";
    std::cout << "8. Assign task to robot\n";
    std::cout << "9. Show task queue\n";
    std::cout << "10. Find first robot below battery threshold\n";
    std::cout << "11. Start timed work on mobile robot\n";
    std::cout << "0. Exit\n";
}

/**
 * @brief Adds a robot to the fleet based on user input.
 *
 * @param fleet Fleet to modify.
 */
void add_robot_menu(Fleet& fleet) {
    std::cout << "\nChoose robot type:\n";
    std::cout << "1. MobileRobot\n";
    std::cout << "2. CleaningRobot\n";
    std::cout << "3. CookingRobot\n";

    int type = read_int("Type: ");

    if (type < 1 || type > 3) {
        std::cout << "Invalid robot type.\n";
        return;
    }

    std::string id = read_non_empty_string("Robot id: ");
    std::string name = read_non_empty_string("Robot name: ");
    int battery = read_int("Battery level: ");

    if (type == 1) {
        double speed = read_double("Speed: ");
        fleet.add(std::make_shared<MobileRobot>(id, name, battery, speed));
        std::cout << "MobileRobot added.\n";
        return;
    }
    else if (type == 2) {
        double speed = read_double("Speed: ");
        int brush_power = read_positive_int("Brush power: ");

        fleet.add(std::make_shared<CleaningRobot>(
            id, name, battery, speed, brush_power
        ));

        std::cout << "CleaningRobot added.\n";
        return;
    }
    else if (type == 3) {
        std::string cooking_method = read_non_empty_string("Cooking method: ");

        fleet.add(std::make_shared<CookingRobot>(
            id, name, battery, cooking_method
        ));

        std::cout << "CookingRobot added.\n";
        return;
    }
}

/**
 * @brief Removes a robot from the fleet.
 *
 * @param fleet Fleet to modify.
 */
void remove_robot_menu(Fleet& fleet) {
    std::string id = read_non_empty_string("Robot id to remove: ");
    fleet.remove(id);
    std::cout << "Robot removed.\n";
}

/**
 * @brief Calls work() on a single robot.
 *
 * @param fleet Fleet containing the robot.
 */
void work_single_robot_menu(const Fleet& fleet) {
    std::string id = read_non_empty_string("Robot id to work: ");
    std::shared_ptr<Robot> robot = fleet.find(id);

    robot->work();
    std::cout << *robot << "\n";
}

/**
 * @brief Calls charge() on a single robot.
 *
 * @param fleet Fleet containing the robot.
 */
void charge_single_robot_menu(const Fleet& fleet) {
    std::string id = read_non_empty_string("Robot id to charge: ");

    std::shared_ptr<Robot> robot = fleet.find(id);

    robot->charge();
    std::cout << *robot << "\n";
}

/**
 * @brief Assigns a task to a robot.
 *
 * @param fleet Fleet to modify.
 */
void assign_task_menu(Fleet& fleet) {
    std::string robot_id = read_non_empty_string("Robot id: ");
    std::string task_name = read_non_empty_string("Task name: ");

    int priority;

    while (true) {
        priority = read_int("Priority from 1 to 5: ");

        if (priority >= 1 && priority <= 5) {
            break;
        }

        std::cout << "Priority must be between 1 and 5.\n";
    }

    Task task{task_name, priority, ""};
    fleet.assign_task(robot_id, task);

    std::cout << "Task assigned.\n";
}

/**
 * @brief Starts timed work on a mobile robot.
 *
 * @param fleet Fleet containing the robot.
 */
void start_timed_work_menu(const Fleet& fleet) {
    std::string id = read_non_empty_string("Robot id: ");
    std::shared_ptr<Robot> robot = fleet.find(id);

    auto mobile_robot = std::dynamic_pointer_cast<MobileRobot>(robot);

    if (!mobile_robot) {
        std::cout << "This robot does not support timed work.\n";
        return;
    }

    int seconds = read_positive_int("Duration in seconds: ");
    mobile_robot->start_work(seconds);

    std::cout << "Timed work started. Please wait until it finishes...\n";
    std::this_thread::sleep_for(std::chrono::seconds(seconds + 1));
}

/**
 * @brief Searches for the first robot below a battery threshold.
 *
 * @param fleet Fleet to search.
 */
void low_battery_menu(const Fleet& fleet) {
    int threshold = read_int("Battery threshold: ");
    fleet.find_first_below_battery(threshold);
}

int main() {
    Fleet fleet;
    bool running = true;

    while (running) {
        print_menu();

        int choice = read_int("Choice: ");

        try {
            switch (choice) {
                case 1:
                    add_robot_menu(fleet);
                    break;

                case 2:
                    remove_robot_menu(fleet);
                    break;

                case 3:
                    std::cout << fleet;
                    break;

                case 4:
                    work_single_robot_menu(fleet);
                    break;

                case 5:
                    fleet.work_all();
                    std::cout << "All robots worked.\n";
                    break;
                
                case 6:
                    charge_single_robot_menu(fleet);
                    break;

                case 7:
                    fleet.charge_all();
                    std::cout << "All robots charged.\n";
                    break;

                case 8:
                    assign_task_menu(fleet);
                    break;

                case 9:
                    fleet.show_tasks();
                    break;

                case 10:
                    low_battery_menu(fleet);
                    break;

                case 11:
                    start_timed_work_menu(fleet);
                    break;

                case 0:
                    running = false;
                    std::cout << "Goodbye.\n";
                    break;

                default:
                    std::cout << "Invalid choice.\n";
                    break;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}