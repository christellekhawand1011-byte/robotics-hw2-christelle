# Part B — Answers

## 1. Why `shared_ptr` in Fleet and not `unique_ptr`? What would break if you switched?

We used `std::shared_ptr<Robot>` because the fleet stores and manages the robots, but other parts of the program can still temporarily reference the same robot, for example in `main.cpp` after calling `find()`.

If I used `std::unique_ptr`, the robot would have only one owner. That means I could not easily copy or return the pointer from `find()`. I would have to move ownership, which would remove the robot from one place when giving it to another.

---

## 2. What container did you use for `robots_`? Why — what is its lookup cost?

I used:

```cpp
std::unordered_map<std::string, std::shared_ptr<Robot>> robots_;
```
The key is the robot id, and the value is the robot pointer.

This is useful because robots need to be found quickly by id. With `unordered_map`, lookup by id is average O(1) (constant time on average), which is faster than searching through a vector one by one.

---

## 3. What container did you use for the task queue? What data structure is it built on, and why is it the right fit for "highest priority first"?

I used:

```cpp
std::priority_queue<Task> tasks_;
```
A `priority_queue` is usually implemented using a heap, with a vector as the underlying container by default.

It is the right fit because we always want the highest-priority task to come first. Since `Task::operator<` compares priorities, the task with the highest priority is placed at the top of the queue.

---

## 4. `operator<<` is declared `friend` inside Robot. What does `friend` give it that a regular method does not? Why can it not be a regular member function?

`friend` gives `operator<<` access to the protected/private data of Robot, such as `id_`, `name_`, `battery_`, and `status_`, since `operator<<` cannot be a regular member function of Robot because the left side of the operation is `std::ostream`.

For example:
```cpp
std::cout << robot;
```
means:
```cpp
operator<<(std::cout, robot);
```
So the function must be a free function, not a method called by the robot. If it were a member function of Robot, the syntax would be more like:
```cpp
robot << std::cout;
```
which is not what we want.

---

## 5. You used `std::find_if` with a lambda. What does that communicate compared to a hand-written loop?

I used `std::find_if` to find the first robot with a battery below a given threshold.

Using `std::find_if` with a lambda makes the intention clearer. It says directly: “find the first element that satisfies this condition.”

A hand-written loop would also work, but `find_if` is more expressive and shows that I am using the STL algorithm made for this kind of search.

---

## 6. `work()` throws if battery is 0. Why use an exception rather than returning false or printing an error?

An exception is used because a robot with 0 battery cannot continue the normal execution of `work()`.

`work()` should either complete its job or clearly signal that something prevented it from working. An empty battery is not just a normal result; it is a problem that the caller must handle.

If I only printed an error inside `work()`, the rest of the program would not really know that the work failed and would continue to execute. 
If I returned `false`, every place that calls `work()` would have to remember to check the return value, or else the program will continue as if nothing went wrong.

Using an exception makes the error harder to ignore. The caller can catch it and decide what to do.

---

## 7. An exception is thrown inside `assign_task` when the robot is not found. Is the fleet's state consistent afterwards?

Yes, the fleet stays consistent.

In `assign_task`, the robot is found first using find(robot_id). If the robot does not exist, find() throws an exception immediately.

Because of that, the task is not pushed into the priority queue and no robot status is changed. So the fleet stays in the same state as before the failed assignment.

---

## 8. `start_work` uses a background thread. What did you do with `stop_` and `worker_`, and why?

`worker_` stores the background thread.

In `start_work`, the thread calls `work()` once per second for the requested duration. After each call, it prints the robot status.

`stop_` is an atomic boolean used to safely tell the thread to stop. It is atomic because the main program and the worker thread may access it at the same time.

Before starting a new thread, I check if the previous one is still joinable. If it is, I stop it and join it. In the destructor, I also stop and join the thread. This prevents the program from ending while the background thread is still running.

---

## 9. **Diamond inheritance ** 

```cpp
class Robot { public: std::string name_ = "base"; };
class MobileRobot      : public Robot {};
class CookingRobot     : public Robot {};
class MaintenanceRobot : public MobileRobot, public CookingRobot {};

int main() {
    MaintenanceRobot m;
    std::cout << m.name_;   // which name_?
}
```
This does not compile because `MaintenanceRobot` gets two separate copies of `Robot`:
- one copy through `MobileRobot`
- one copy through `CookingRobot`

So when we write:
```cpp
m.name_;
```
C++ does not know which `name_` we mean. The one from the `MobileRobot` side or the one from the `CookingRobot` side.

This problem is called the diamond inheritance problem.

C++ solves it using virtual inheritance:
```cpp
class Robot {
public:
    std::string name_ = "base";
};

class MobileRobot : virtual public Robot {};
class CookingRobot : virtual public Robot {};

class MaintenanceRobot : public MobileRobot, public CookingRobot {};

int main() {
    MaintenanceRobot m;
    std::cout << m.name_ << "\n";
    return 0;
}
```
With `virtual public Robot`, both `MobileRobot` and `CookingRobot` share one common `Robot` base inside `MaintenanceRobot`.

So instead of having two copies of `Robot`, `MaintenanceRobot` has only one shared copy.