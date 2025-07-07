#include "Scheduler.hpp"
#include "TaskGraph.hpp"
#include "ThreadPool.hpp"
#include "Expression.hpp"
#include <cassert>
#include <variant>
#include <any>
#include <vector>
#include <future>
#include <thread>

std::variant<int, double> Scheduler::serialSchedule()
{
    auto &leaves = m_task_graph.m_leaves;
    std::any result;

    while (!leaves.empty())
    {
        auto leaf = std::move(leaves.front());
        leaves.pop_front();
        result = leaf.execute();
        auto targetExpr = leaf.getTargetExpr();
        auto par = targetExpr->getParent();
        if (checkIfParentReady(par))
        {
            auto &op = std::get<Expression::Operation>(par->value);
            leaves.push_back(createTask(op, par));
        }
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

std::variant<int, double> Scheduler::parallelSchedule()
{
    ThreadPool pool(std::thread::hardware_concurrency());
    auto &leaves = m_task_graph.m_leaves;
    std::any result;
    while (!leaves.empty())
    {
        std::vector<std::pair<std::future<std::any>, std::shared_ptr<Expression>>> futures;
        while (!leaves.empty())
        {
            auto leaf = std::move(leaves.front());
            leaves.pop_front();
            auto targetExpr = leaf.getTargetExpr();
            futures.push_back(std::make_pair(pool.enqueueTask(std::move(leaf)), std::move(targetExpr)));
        }
        for (auto &future : futures)
        {
            result = future.first.get();
        }
        for (auto &future : futures)
        {
            auto par = future.second->getParent();
            if (checkIfParentReady(par))
            {
                auto &op = std::get<Expression::Operation>(par->value);
                leaves.push_back(createTask(op, par));
            }
        }
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

bool Scheduler::checkIfParentReady(std::shared_ptr<Expression> &par)
{
    if (par)
    {
        if (!par->isOperation())
            throw std::runtime_error("Parent is not an operation");
        auto &op = std::get<Expression::Operation>(par->value);
        bool allAtoms = true;
        for (auto &operand : op.operands)
        {
            if (operand->isOperation())
            {
                allAtoms = false;
                break;
            }
        }
        return allAtoms;
    }
    return false;
}

Task Scheduler::createTask(Expression::Operation &op, std::shared_ptr<Expression> &par)
{
    auto numOperands = op.operands.size();

    assert(numOperands == 1 || numOperands == 2);
    std::unique_ptr<Ops> taskOp;
    if (numOperands == 1)
    {
        // unary operation
        auto value = Helper::extractAtomValue(*op.operands[0]);
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
        auto value1 = Helper::extractAtomValue(*op.operands[0]);
        auto value2 = Helper::extractAtomValue(*op.operands[1]);
        taskOp = std::visit(
            [&](auto value1, auto value2)
            {
                return Helper::createBinaryOp(op.op, value1, value2);
            },
            value1, value2);
    }
    return Task(Task::s_id_counter++, std::move(taskOp), std::move(par));
}