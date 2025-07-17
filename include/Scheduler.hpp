#pragma once

#include "TaskGraph.hpp"
#include "ThreadPool.hpp"
#include "Expression.hpp"
#include "Task.hpp"
#include <variant>
#include <memory>

struct Scheduler
{
    TaskGraph &m_task_graph;
    static std::unique_ptr<ThreadPool> m_pool;
    const std::size_t num_threads = std::thread::hardware_concurrency();

    explicit Scheduler(TaskGraph &task_graph) : m_task_graph(task_graph) {
        if (!m_pool)
            m_pool = std::make_unique<ThreadPool>(num_threads);
    }
    [[nodiscard]] std::variant<int, double> serialSchedule() const;
    [[nodiscard]] std::variant<int, double> parallelSchedule() const;

    static bool checkIfParentReady(const std::shared_ptr<Expression> &par);
    static Task createTask(Expression::Operation &op, std::shared_ptr<Expression> &par);
};