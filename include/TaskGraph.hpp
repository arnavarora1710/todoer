#pragma once

#include "Task.hpp"
#include "Expression.hpp"
#include <deque>
#include <unordered_map>
#include <variant>

typedef std::unordered_map<std::string, std::variant<int, double>> VariableMap;

// Want to be able to create a TaskGraph from an Expression
struct TaskGraph
{
    // the "leaves" would be the index of the task in the graph
    // for example, we can safely evaluate a leaf (2, +, 3) or a leaf (2, -)
    std::deque<Task> m_leaves{};
    VariableMap &m_variables;

    TaskGraph() = delete;
    // only need to construct from an expression
    TaskGraph(Expression &expr, VariableMap &variables);
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