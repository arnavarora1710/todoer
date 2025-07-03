#include "TaskGraph.hpp"
#include <cassert>

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
        catch (const std::exception &e)
        {
            throw std::runtime_error("An error occurred while parsing the atom value: " + atom_value +
                                     "\nPress Ctrl+C or type 'exit' to exit the program");
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

std::deque<Task> getLeaves(Expression &expr)
{
    std::deque<Task> leaves;

    if (expr.isAtom())
    {
        // No task required - this will only happen if the expression is a single atom
        auto value = Helper::extractAtomValue(expr);
        auto taskOp = std::visit(
            [&](auto value)
            {
                return Helper::createNoOp(value);
            },
            value);
        std::shared_ptr<Expression> expr_ptr(&expr, [](Expression *) {});
        Task task(Task::s_id_counter++, std::move(taskOp), expr_ptr);
        leaves.push_back(std::move(task));
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
        if (operand->isAtom())
        {
            auto value = Helper::extractAtomValue(*operand);
            auto taskOp = std::visit(
                [&](auto value)
                {
                    return Helper::createUnaryOp(op.op, value);
                },
                value);
            std::shared_ptr<Expression> expr_ptr(&expr, [](Expression *) {});
            Task task(Task::s_id_counter++, std::move(taskOp), expr_ptr);
            leaves.push_back(std::move(task));
        }
        else
        {
            // Recursively get leaves from operand
            Helper::appendLeaves(leaves, getLeaves(*operand));
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
                [&](auto value1, auto value2)
                {
                    return Helper::createBinaryOp(op.op, value1, value2);
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
                Helper::appendLeaves(leaves, getLeaves(*operand1));
            }
            if (operand2->isOperation())
            {
                Helper::appendLeaves(leaves, getLeaves(*operand2));
            }
        }
    }

    return leaves;
}

TaskGraph::TaskGraph(Expression &expr)
{
    m_leaves = getLeaves(expr);
}