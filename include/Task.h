#pragma once

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include "Ops.h"

// this structure should store task metadata as well as
// a pointer to the operation that the task will execute

// idea is to make multiple simple functions that can be executed
// example: add, subtract, multiply, divide, etc.
class Task
{
private:
    // needs the id of the task + operation to execute
    const std::size_t m_id{};
    const std::unique_ptr<Ops> m_operation{};

public:
    explicit Task(std::size_t id, std::unique_ptr<Ops> &&operation) : m_id{id}, m_operation{std::move(operation)} {}

    std::size_t getId() const { return m_id; }

    std::any execute() const
    {
        if (!m_operation)
        {
            throw std::runtime_error("No operation assigned to task with id: " + std::to_string(m_id));
        }
        // execute the function and return the result
        return m_operation->execute();
    }
};
