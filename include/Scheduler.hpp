#pragma once

#include "TaskGraph.hpp"
#include <variant>

class Scheduler
{
private:
    TaskGraph &m_task_graph;

public:
    Scheduler(TaskGraph &task_graph) : m_task_graph(task_graph) {}
    std::variant<int, double> schedule(Expression &expr);
};