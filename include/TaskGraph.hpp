#pragma once

#include "Task.hpp"
#include "Expression.hpp"
#include "ops/Ops.hpp"
#include "ops/NoOps.hpp"
#include "ops/UnaryOps.hpp"
#include "ops/BinaryOps.hpp"
#include <deque>
#include <variant>
#include <memory>
#include <functional>
#include <stdexcept>
#include <cmath>

// Forward declarations
class Ops;

namespace Helper
{
    // Helper function to extract value from atom
    std::variant<int, double> extractAtomValue(const Expression &atom);

    // Helper function to create no-op
    template <typename T>
    std::unique_ptr<Ops> createNoOp(T value)
    {
        return make_no_ops(value);
    }

    // Helper function to create unary operation
    template <typename T>
    std::unique_ptr<Ops> createUnaryOp(const std::string &op, T value)
    {
        switch (op[0])
        {
        case '+':
            return make_unary_ops(std::identity{}, value);
        case '-':
            return make_unary_ops(std::negate<T>{}, value);
        default:
            throw std::runtime_error("Unknown unary operator: " + op);
        }
    }

    // Helper function to create binary operation
    template <typename T, typename U>
    std::unique_ptr<Ops> createBinaryOp(const std::string &op, T value1, U value2)
    {
        switch (op[0])
        {
        case '+':
            return make_binary_ops(std::plus<decltype(value1 + value2)>{}, value1, value2);
        case '-':
            return make_binary_ops(std::minus<decltype(value1 - value2)>{}, value1, value2);
        case '*':
            return make_binary_ops(std::multiplies<decltype(value1 * value2)>{}, value1, value2);
        case '/':
            return make_binary_ops(std::divides<decltype(value1 / value2)>{}, value1, value2);
        case '^':
            return make_binary_ops([](auto a, auto b)
                                   { return std::pow(a, b); }, value1, value2);
        case '&':
            if (std::is_same_v<decltype(value1), int> && std::is_same_v<decltype(value2), int>)
                return make_binary_ops([](auto a, auto b)
                                       { return a & b; },
                                       static_cast<int>(value1),
                                       static_cast<int>(value2));
            else
                throw std::runtime_error("Invalid operands for bitwise AND operator, both operands must be integers");
        case '|':
            if (std::is_same_v<decltype(value1), int> && std::is_same_v<decltype(value2), int>)
                return make_binary_ops([](auto a, auto b)
                                       { return a | b; },
                                       static_cast<int>(value1),
                                       static_cast<int>(value2));
            else
                throw std::runtime_error("Invalid operands for bitwise OR operator, both operands must be integers");
        default:
            throw std::runtime_error("Unknown binary operator: " + op);
        }
    }

    // Helper function to append leaves from one deque to another
    void appendLeaves(std::deque<Task> &target, std::deque<Task> &&source);
}

// Want to be able to create a TaskGraph from an Expression
struct TaskGraph
{
    // the "leaves" would be the index of the task in the graph
    // for example, we can safely evaluate a leaf (2, +, 3) or a leaf (2, -)
    std::deque<Task> m_leaves{};

    TaskGraph() = delete;
    // only need to construct from an expression
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

std::deque<Task> getLeaves(Expression &expr);