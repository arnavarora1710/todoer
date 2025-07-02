#pragma once

#include "Task.hpp"
#include "Expression.hpp"
#include <deque>

// Want to be able to create a TaskGraph from an Expression
struct TaskGraph
{
    // the "leaves" would be the index of the task in the graph
    // for example, we can safely evaluate a leaf (2, +, 3) or a leaf (2, -)
    std::deque<Task> m_leaves{};

    // only need to construct from an expression
    TaskGraph() = default;
    TaskGraph(Expression &expr);
    // disable copy and move constructors / assignment operators
    // because we don't want to copy/move the graph
    TaskGraph(const TaskGraph &) = delete;
    TaskGraph(TaskGraph &&) = delete;
    TaskGraph &operator=(const TaskGraph &) = delete;
    TaskGraph &operator=(TaskGraph &&) = delete;
    // default destructor - just deallocate the graph and queue
    ~TaskGraph() = default;
};

std::deque<Task> getLeaves(const Expression &expr);