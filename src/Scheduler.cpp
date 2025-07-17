#include "Scheduler.hpp"
#include "TaskGraph.hpp"
#include "Expression.hpp"
#include <cassert>
#include <variant>
#include <vector>
#include <future>

// Define the static member variable
std::unique_ptr<ThreadPool> Scheduler::m_pool = nullptr;

std::variant<int, double> Scheduler::serialSchedule() const {
    auto &leaves = m_task_graph.m_leaves;
    std::variant<int, double> result{0};

    while (!leaves.empty())
    {
        auto leaf = std::move(leaves.front());
        leaves.pop_front();
        result = leaf.execute();
        const auto targetExpr = leaf.getTargetExpr();
        if (auto par = targetExpr->getParent(); checkIfParentReady(par))
        {
            auto &op = std::get<Expression::Operation>(par->value);
            leaves.push_back(createTask(op, par));
        }
    }
    return result;
}

std::variant<int, double> Scheduler::parallelSchedule() const {
    auto &leaves = m_task_graph.m_leaves;
    std::variant<int, double> result{0};

    std::vector<std::pair<std::future<std::variant<int, double>>, std::shared_ptr<Expression>>> futures;
    while (!leaves.empty()) {
        futures.reserve(leaves.size());

        // Schedule all tasks in this round
        while (!leaves.empty()) {
            auto leaf = std::move(leaves.front());
            leaves.pop_front();
            auto targetExpr = leaf.getTargetExpr();
            futures.emplace_back(m_pool->enqueueTask(std::move(leaf)), std::move(targetExpr));
        }

        // Wait for all tasks and process results
        while (!futures.empty()) {
            auto [future, targetExpr] = std::move(futures.back());
            futures.pop_back();
            result = future.get();
            if (auto par = targetExpr->getParent(); checkIfParentReady(par)) {
                auto &op = std::get<Expression::Operation>(par->value);
                leaves.push_back(createTask(op, par));
            }
        }
    }

    return result;
}

bool Scheduler::checkIfParentReady(const std::shared_ptr<Expression> &par)
{
    if (!par)
        return false;
    if (!par->isOperation())
        throw std::runtime_error("Parent is not an operation");

    const auto &[op, operands] = std::get<Expression::Operation>(par->value);
    return std::none_of(operands.begin(), operands.end(),
                        [](const auto &operand) { return operand->isOperation(); });
}

Task Scheduler::createTask(Expression::Operation &op, std::shared_ptr<Expression> &par)
{
    const auto numOperands = op.operands.size();

    assert(numOperands == 1 || numOperands == 2);
    std::unique_ptr<Ops> taskOp;
    if (numOperands == 1)
    {
        // unary operation
        auto value = Helper::extractAtomValue(*op.operands[0]);
        taskOp = std::visit(
            [&](auto value_param)
            {
                return Helper::createUnaryOp(op.op, value_param);
            },
            value);
    }
    else
    {
        // binary operation
        auto value1 = Helper::extractAtomValue(*op.operands[0]);
        auto value2 = Helper::extractAtomValue(*op.operands[1]);
        taskOp = std::visit(
            [&](auto value1_param, auto value2_param)
            {
                return Helper::createBinaryOp(op.op, value1_param, value2_param);
            },
            value1, value2);
    }
    return Task(Task::s_id_counter++, std::move(taskOp), std::move(par));
}