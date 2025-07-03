#include "Task.hpp"
#include "TaskGraph.hpp"
#include "Expression.hpp"
#include "Lexer.hpp"
#include "Scheduler.hpp"
#include <iostream>
#include <any>
#include <functional>
#include <unordered_map>
#include <variant>

std::variant<int, double> evaluate(Expression &expr)
{
    TaskGraph tg(expr);
    Scheduler scheduler(tg);
    return scheduler.schedule(expr);
}

std::vector<std::string> splitOnEqual(std::string_view input)
{
    std::vector<std::string> result;
    std::string_view::size_type pos = 0;
    while (pos != std::string_view::npos)
    {
        auto equal_pos = input.find('=', pos);
        if (equal_pos != std::string_view::npos)
        {
            result.push_back(std::string(input.substr(pos, equal_pos - pos)));
            pos = equal_pos + 1;
        }
        else
        {
            result.push_back(std::string(input.substr(pos)));
            break;
        }
    }
    return result;
}

std::string interpret(std::string_view input, VariableMap &variables)
{
    auto split = splitOnEqual(input);
    assert(split.size() <= 2);
    if (split.size() == 2)
    {
        auto lhs = split[0];
        while (lhs.back() == ' ')
            lhs.pop_back();
        Expression rhs = from_string(split[1], variables);
        variables[lhs] = evaluate(rhs);
    }
    else
    {
        Expression new_expr = from_string(input, variables);
        auto result = evaluate(new_expr);
        return std::visit([&](auto result)
                          { return std::to_string(result) + "\n"; },
                          result);
    }
    return "";
}

int main()
{
    VariableMap variables;
    while (true)
    {
        std::cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit")
            break;
        std::cout << interpret(input, variables);
    }
    return 0;
}
