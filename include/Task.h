#ifndef TASK_H
#define TASK_H

#include <string>

// this structure should store task metadata as well as
// a pointer to the program that the task will execute

// idea is to make multiple simple programs that can be executed
// example: add.cpp, subtract.cpp, multiply.cpp, divide.cpp, etc.
class Task
{
private:
    const int id{};
    const std::string programPath{};

public:
    Task(int id, std::string programPath);

    // need to template this as the program can return any type as result
    template <typename T>
    T execute();
};

#endif