#include "TaskGraph.hpp"
#include "Ops.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

namespace Helper
{
    // Helper function to extract integer value from atom
    int extractAtomValue(const Expression::Atom &atom)
    {
        return std::stoi(atom.value);
    }

    // Helper function to create unary operation
    std::unique_ptr<Ops> createUnaryOp(char op, int value)
    {
        switch (op)
        {
        case '+':
            return make_unary_ops(std::identity{}, value);
        case '-':
            return make_unary_ops(std::negate<int>{}, value);
        default:
            throw std::runtime_error("Unknown unary operator: " + std::string(1, op));
        }
    }

    // Helper function to create binary operation
    std::unique_ptr<Ops> createBinaryOp(char op, int value1, int value2)
    {
        switch (op)
        {
        case '+':
            return make_binary_ops(std::plus<int>{}, value1, value2);
        case '-':
            return make_binary_ops(std::minus<int>{}, value1, value2);
        case '*':
            return make_binary_ops(std::multiplies<int>{}, value1, value2);
        case '/':
            return make_binary_ops(std::divides<int>{}, value1, value2);
        default:
            throw std::runtime_error("Unknown binary operator: " + std::string(1, op));
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
            int value = Helper::extractAtomValue(std::get<Expression::Atom>(operand.value));
            auto taskOp = Helper::createUnaryOp(op.op, value);
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
            int value1 = Helper::extractAtomValue(std::get<Expression::Atom>(operand1.value));
            int value2 = Helper::extractAtomValue(std::get<Expression::Atom>(operand2.value));
            auto taskOp = Helper::createBinaryOp(op.op, value1, value2);
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

TaskGraph::TaskGraph(Expression &expr)
{
    m_leaves = getLeaves(expr);
}