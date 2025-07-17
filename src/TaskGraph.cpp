#include "TaskGraph.hpp"
#include <cassert>
#include <memory>

namespace Helper
{
    // Helper function to extract value from atom
    std::variant<int, double> extractAtomValue(const Expression &atom)
    {
        auto &atom_value = std::get<Expression::Atom>(atom.value).value;
        try
        {
            if (atom_value.find('.') != std::string::npos)
                return std::stod(atom_value);
            else
                return std::stoi(atom_value);
        }
        catch ([[maybe_unused]] const std::exception &e)
        {
            throw std::runtime_error("An error occurred while parsing the atom value: " + atom_value +
                                     "\nPress Ctrl+C or type 'exit' to exit the program");
        }
    }
}

void getLeaves(Expression &expr, std::deque<Task>& leaves)
{
    if (expr.isAtom())
    {
        // No task required - this will only happen if the expression is a single atom
        auto value = Helper::extractAtomValue(expr);
        auto taskOp = std::visit(
            [&](auto value_param)
            {
                return Helper::createNoOp(value_param);
            },
            value);
        std::shared_ptr<Expression> expr_ptr(&expr, [](Expression *) {});
        Task task(Task::s_id_counter++, std::move(taskOp), expr_ptr);
        leaves.push_back(std::move(task));
        return;
    }

    // Handle operation expression
    const auto &op = std::get<Expression::Operation>(expr.value);
    const std::size_t num_operands = op.operands.size();
    assert(num_operands > 0 && num_operands <= 2);

    if (num_operands == 1)
    {
        // Unary operation
        const auto &operand = op.operands[0];
        if (operand->isAtom())
        {
            auto value = Helper::extractAtomValue(*operand);
            auto taskOp = std::visit(
                [&](auto value_param)
                {
                    return Helper::createUnaryOp(op.op, value_param);
                },
                value);
            std::shared_ptr<Expression> expr_ptr(&expr, [](Expression *) {});
            Task task(Task::s_id_counter++, std::move(taskOp), expr_ptr);
            leaves.push_back(std::move(task));
        }
        else
        {
            // Recursively get leaves from operand
            getLeaves(*operand, leaves);
        }
    }
    else
    {
        // Binary operation
        const auto &operand1 = op.operands[0];
        const auto &operand2 = op.operands[1];

        if (operand1->isAtom() && operand2->isAtom())
        {
            // Both operands are atoms
            auto value1 = Helper::extractAtomValue(*operand1);
            auto value2 = Helper::extractAtomValue(*operand2);
            auto taskOp = std::visit(
                [&](auto value1_param, auto value2_param)
                {
                    return Helper::createBinaryOp(op.op, value1_param, value2_param);
                },
                value1,
                value2);
            std::shared_ptr<Expression> expr_ptr(&expr, [](Expression *) {});
            Task task(Task::s_id_counter++, std::move(taskOp), expr_ptr);
            leaves.push_back(std::move(task));
        }
        else
        {
            // Handle mixed operand types
            if (operand1->isOperation())
            {
                getLeaves(*operand1, leaves);
            }
            if (operand2->isOperation())
            {
                getLeaves(*operand2, leaves);
            }
        }
    }
}

TaskGraph::TaskGraph(Expression &expr)
{
    getLeaves(expr, m_leaves);
}