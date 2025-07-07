#pragma once

#include "TaskGraph.hpp"
#include "Expression.hpp"
#include "Task.hpp"
#include <variant>
#include <memory>

class Scheduler
{
private:
    TaskGraph &m_task_graph;

public:
    Scheduler(TaskGraph &task_graph) : m_task_graph(task_graph) {}
    std::variant<int, double> serialSchedule();
    std::variant<int, double> parallelSchedule();
    bool checkIfParentReady(std::shared_ptr<Expression> &par);
    Task createTask(Expression::Operation &op, std::shared_ptr<Expression> &par);
};