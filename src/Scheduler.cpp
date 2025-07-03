#include "Scheduler.hpp"
#include "TaskGraph.hpp"
#include "Expression.hpp"
#include <cassert>
#include <variant>
#include <iostream>
#include <any>

std::variant<int, double> Scheduler::schedule(Expression &expr)
{
    auto &leaves = m_task_graph.m_leaves;
    std::any result;
    while (!leaves.empty())
    {
        auto &leaf = leaves.front();
        result = leaf.execute();
        auto targetExpr = leaf.getTargetExpr();
        auto par = targetExpr->getParent();
        if (par)
        {
            // the parent has to be an operation
            assert(par->isOperation());
            auto &op = std::get<Expression::Operation>(par->value);
            auto numOperands = op.operands.size();
            bool allAtoms = true;
            for (auto &operand : op.operands)
            {
                if (operand.isOperation())
                {
                    allAtoms = false;
                    break;
                }
            }
            if (allAtoms)
            {
                assert(numOperands == 1 || numOperands == 2);
                std::unique_ptr<Ops> taskOp;
                if (numOperands == 1)
                {
                    // unary operation
                    auto value = Helper::extractAtomValue(op.operands[0]);
                    taskOp = std::visit(
                        [&](auto value)
                        {
                            return Helper::createUnaryOp(op.op, value);
                        },
                        value);
                }
                else
                {
                    // binary operation
                    auto value1 = Helper::extractAtomValue(op.operands[0]);
                    auto value2 = Helper::extractAtomValue(op.operands[1]);
                    taskOp = std::visit(
                        [&](auto value1, auto value2)
                        {
                            return Helper::createBinaryOp(op.op, value1, value2);
                        },
                        value1, value2);
                }
                leaves.push_back(Task(Task::s_id_counter++, std::move(taskOp), const_cast<Expression *>(par)));
            }
        }
        leaves.pop_front();
    }
    try
    {
        return std::any_cast<int>(result);
    }
    catch (const std::bad_any_cast &)
    {
        return std::any_cast<double>(result);
    }
}