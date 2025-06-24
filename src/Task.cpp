#include "../include/Task.h"

Task::Task(int id, std::string programPath) : id(id), programPath(programPath) {}

template <typename T>
T Task::execute()
{
    // execute the program
    // return the result
    return T{};
}