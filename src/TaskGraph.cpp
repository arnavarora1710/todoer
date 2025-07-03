#include "TaskGraph.hpp"
#include "Ops.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

namespace Helper
{
    // Helper function to extract value from atom
    std::variant<int, double> extractAtomValue(const Expression::Atom &atom)
    {
        if (atom.value.find('.') != std::string::npos)
            return std::stod(atom.value);
        else
            return std::stoi(atom.value);
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
        default:
            throw std::runtime_error("Unknown binary operator: " + op);
        }
    }

    // Helper function to append leaves from one deque to another
    void appendLeaves(std::deque<Task> &target, std::deque<Task> &&source)
    {
        target.insert(target.end(),
                      std::make_move_iterator(source.begin()),
                      std::make_move_iterator(source.end()));
    }
}

std::deque<Task> getLeaves(const Expression &expr)
{
    std::deque<Task> leaves;

    if (expr.isAtom())
    {
        // No task required - this will only happen if the expression is a single atom
        return leaves;
    }

    // Handle operation expression
    const auto &op = std::get<Expression::Operation>(expr.value);
    const std::size_t num_operands = op.operands.size();
    assert(num_operands > 0 && num_operands <= 2);

    if (num_operands == 1)
    {
        // Unary operation
        const auto &operand = op.operands[0];
        if (operand.isAtom())
        {
            auto value = Helper::extractAtomValue(std::get<Expression::Atom>(operand.value));
            auto taskOp = std::visit(
                [&](auto value)
                {
                    return Helper::createUnaryOp(op.op, value);
                },
                value);
            Task task(Task::s_id_counter++, std::move(taskOp), const_cast<Expression *>(&expr));
            leaves.push_back(std::move(task));
        }
        else
        {
            // Recursively get leaves from operand
            Helper::appendLeaves(leaves, getLeaves(const_cast<const Expression &>(operand)));
        }
    }
    else
    {
        // Binary operation
        const auto &operand1 = op.operands[0];
        const auto &operand2 = op.operands[1];

        if (operand1.isAtom() && operand2.isAtom())
        {
            // Both operands are atoms
            auto value1 = Helper::extractAtomValue(std::get<Expression::Atom>(operand1.value));
            auto value2 = Helper::extractAtomValue(std::get<Expression::Atom>(operand2.value));
            auto taskOp = std::visit(
                [&](auto value1, auto value2)
                {
                    return Helper::createBinaryOp(op.op, value1, value2);
                },
                value1,
                value2);
            Task task(Task::s_id_counter++, std::move(taskOp), const_cast<Expression *>(&expr));
            leaves.push_back(std::move(task));
        }
        else
        {
            // Handle mixed operand types
            if (operand1.isOperation())
            {
                Helper::appendLeaves(leaves, getLeaves(const_cast<const Expression &>(operand1)));
            }
            if (operand2.isOperation())
            {
                Helper::appendLeaves(leaves, getLeaves(const_cast<const Expression &>(operand2)));
            }
        }
    }

    return leaves;
}

TaskGraph::TaskGraph(Expression &expr, std::unordered_map<std::string, std::variant<int, double>> &variables)
    : m_variables(variables)
{
    m_leaves = getLeaves(expr);
}