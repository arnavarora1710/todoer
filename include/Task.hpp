#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include "ops/Ops.hpp"
#include "Expression.hpp"
#include <mutex>
#include <utility>

// this structure should store task metadata as well as
// a pointer to the operation that the task will execute

// idea is to make multiple simple functions that can be executed
// example: add, subtract, multiply, divide, etc.
struct Task
{
    // needs the id of the task + operation to execute
    std::size_t m_id{};
    std::unique_ptr<Ops> m_operation{};
    std::shared_ptr<Expression> m_target_expr{}; // pointer to the expression node this task will modify
    inline static std::size_t s_id_counter = 0;
    inline static std::mutex s_mutex{};

    explicit Task(const std::size_t id, std::unique_ptr<Ops> &&operation,
                  std::shared_ptr<Expression> target_expr = std::make_shared<Expression>())
                  : m_id{id},
                    m_operation{std::move(operation)},
                    m_target_expr{std::move(target_expr)} {}

    [[nodiscard]] std::size_t getId() const { return m_id; }
    std::unique_ptr<Ops> &getOperation() { return m_operation; }
    std::shared_ptr<Expression> &getTargetExpr() { return m_target_expr; }

    [[nodiscard]] std::variant<int, double> execute() const
    {
        if (!m_operation)
        {
            throw std::runtime_error("No operation assigned to task with id: " + std::to_string(m_id));
        }

        // execute the function and get the result
        const auto result = m_operation->execute();

        // if we have a target expression, update it with the result
        if (m_target_expr)
        {
            // Handle both int and double results

            // Replace the target expression with an atom containing the result
            const std::string result_str = std::visit([] (auto&& value) {
                return std::to_string(value);
            }, result);

            {
                std::unique_lock lock(s_mutex);
                m_target_expr->value = Expression::Atom{result_str};
            }
        }

        return result;
    }
};
