#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "TaskGraph.h"

class Interpreter
{
private:
    // the interpreter will be responsible for interpreting the task graph
    // and executing the tasks in the correct order
    TaskGraph *taskGraph{};

    // also need to map each task_id to a task object
    std::unordered_map<int, Task> taskMap{};

public:
    Interpreter(TaskGraph *taskGraph);
};

#endif