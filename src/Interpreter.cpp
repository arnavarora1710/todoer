#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Expression.hpp"
#include "TaskGraph.hpp"
#include "Scheduler.hpp"
#include <vector>
#include <string>
#include <variant>

std::vector<std::string> splitOnEqual(std::string_view input)
{
    std::vector<std::string> result;
    std::string_view::size_type pos = 0;
    while (pos != std::string_view::npos)
    {
        auto equal_pos = input.find('=', pos);
        if (equal_pos != std::string_view::npos)
        {
            result.emplace_back(input.substr(pos, equal_pos - pos));
            pos = equal_pos + 1;
        }
        else
        {
            result.emplace_back(input.substr(pos));
            break;
        }
    }
    return result;
}

namespace Interpreter
{
    value evaluate(const std::shared_ptr<Expression> &expr, const Mode mode)
    {
        TaskGraph task_graph(*expr);
        const Scheduler scheduler(task_graph);
        if (mode == Mode::Serial)
            return scheduler.serialSchedule();
        return scheduler.parallelSchedule();
    }

    std::string interpret(std::string_view input, VariableMap &variables, Mode mode)
    {
        const auto split = splitOnEqual(input);
        std::variant<int, double> result;
        if (split.size() > 2)
        {
            return "Invalid input";
        }
        if (split.size() == 2)
        {
            auto lhs = split[0];
            while (lhs.back() == ' ')
                lhs.pop_back();
            try
            {
                auto rhs = from_string(split[1], variables);
                result = evaluate(rhs, mode);
                variables[lhs] = result;
            }
            catch (const std::exception &e)
            {
                return e.what();
            }
        }
        else
        {
            try
            {
                auto expr = from_string(input, variables);
                result = evaluate(expr, mode);
            }
            catch (const std::exception &e)
            {
                return e.what();
            }
            return std::visit([&](auto answer)
                              { return std::to_string(answer); },
                              result);
        }
        return "";
    }

    value interpret_numeric(std::string_view input, VariableMap &variables, const Mode mode)
    {
        const auto split = splitOnEqual(input);
        if (split.size() > 2)
        {
            throw std::runtime_error("Invalid input");
        }
        if (split.size() == 2)
        {
            auto lhs = split[0];
            while (lhs.back() == ' ')
                lhs.pop_back();
            const auto rhs = from_string(split[1], variables);
            const value result = evaluate(rhs, mode);
            variables[lhs] = result;
            return result;
        }
        const auto expr = from_string(input, variables);
        return evaluate(expr, mode);
    }
}
