#pragma once

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include "Ops.hpp"
#include "Expression.hpp"
#include <iostream>

// this structure should store task metadata as well as
// a pointer to the operation that the task will execute

// idea is to make multiple simple functions that can be executed
// example: add, subtract, multiply, divide, etc.
class Task
{
private:
    // needs the id of the task + operation to execute
    std::size_t m_id{};
    std::unique_ptr<Ops> m_operation{};
    Expression *m_target_expr{}; // pointer to the expression node this task will modify

public:
    inline static std::size_t s_id_counter = 0;
    explicit Task(std::size_t id, std::unique_ptr<Ops> &&operation, Expression *target_expr = nullptr)
        : m_id{id}, m_operation{std::move(operation)}, m_target_expr{target_expr} {}

    std::size_t getId() const { return m_id; }
    std::unique_ptr<Ops> &getOperation() { return m_operation; }
    Expression *getTargetExpr() const { return m_target_expr; }

    std::any execute() const
    {
        if (!m_operation)
        {
            throw std::runtime_error("No operation assigned to task with id: " + std::to_string(m_id));
        }

        // execute the function and get the result
        std::any result = m_operation->execute();

        // if we have a target expression, update it with the result
        if (m_target_expr)
        {
            // Handle both int and double results
            std::string result_str;
            try
            {
                result_str = std::to_string(std::any_cast<int>(result));
            }
            catch (const std::bad_any_cast &)
            {
                try
                {
                    result_str = std::to_string(std::any_cast<double>(result));
                }
                catch (const std::bad_any_cast &)
                {
                    throw std::runtime_error("Unsupported result type in task execution");
                }
            }

            // Replace the target expression with an atom containing the result
            m_target_expr->value = Expression::Atom{result_str};
        }

        return result;
    }
};
