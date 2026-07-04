# Robot Fleet Manager

## Homework description

This homework is a C++ robot fleet manager.

It uses object-oriented programming to model different types of robots:

- `Robot` is the abstract base class.
- `MobileRobot` inherits from `Robot`.
- `CleaningRobot` inherits from `MobileRobot`.
- `CookingRobot` inherits directly from `Robot`.

The homework also includes:

- `Fleet`, which manages a collection of robots.
- `Task`, which describes a job assigned to a robot.
- An interactive menu in `main.cpp`.

The fleet can add robots, remove robots, find robots by id, assign tasks, show the task queue, make robots work, charge robots, and start timed work for mobile robots.

## Main features

- Add `MobileRobot`, `CleaningRobot`, or `CookingRobot`.
- Remove a robot by id.
- Show all robots in the fleet.
- Make one robot work.
- Make all robots work.
- Charge one robot or all robots.
- Assign tasks to robots.
- Show tasks by priority.
- Start timed work using a background thread.
- Find the first robot below a battery threshold using `std::find_if` and a lambda.
- Handle invalid input and exceptions without crashing.

## File structure

```text
robotics-hw2-christelle/
├── include/
│   ├── robot.hpp
│   ├── mobile_robot.hpp
│   ├── cleaning_robot.hpp
│   ├── cooking_robot.hpp
│   ├── task.hpp
│   └── fleet.hpp
├── src/
│   ├── robot.cpp
│   ├── mobile_robot.cpp
│   ├── cleaning_robot.cpp
│   ├── cooking_robot.cpp
│   ├── task.cpp
│   └── fleet.cpp
├── main.cpp
├── compile.sh
├── README.md
├── ANSWERS.md
└── ENV_CHECK.md
```

## How to compile

Run:

```bash
./compile.sh
```

The compile script uses:

```bash
g++ -std=c++17 -Wall -Wextra src/*.cpp main.cpp -I include -o fleet_app
```

## How to run

After compiling, run:

```bash
./fleet_app
```

## Problems faced and solutions

One problem I faced was in `work_all()`. If one robot had an empty battery, it threw an exception and stopped the function before the remaining robots could work. I solved this by catching exceptions inside the loop, so one robot failing does not stop the others.

I also had an issue with timed work. Since `start_work()` uses a background thread, the robot was printing while the menu was waiting for input. I solved this by making the menu wait until timed work finishes before showing the menu again.