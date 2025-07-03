#include "Task.hpp"
#include "TaskGraph.hpp"
#include "Expression.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <any>
#include <functional>
#include <unordered_map>
#include <variant>

std::variant<int, double> evaluate(Expression &expr, std::unordered_map<std::string, std::variant<int, double>> &variables)
{
    TaskGraph tg(expr, variables);
    while (!tg.m_leaves.empty())
    {
        auto &leaf = tg.m_leaves.front();
        auto result = leaf.execute();
        tg.m_leaves.clear();
        auto leaves = getLeaves(expr);
        tg.m_leaves = std::move(leaves);
        if (tg.m_leaves.empty())
        {
            // Handle both int and double results
            try
            {
                return std::any_cast<int>(result);
            }
            catch (const std::bad_any_cast &)
            {
                try
                {
                    return std::any_cast<double>(result);
                }
                catch (const std::bad_any_cast &)
                {
                    throw std::runtime_error("Unsupported result type");
                }
            }
            break;
        }
    }
    assert(expr.isAtom());
    auto atom = std::get<Expression::Atom>(expr.value);
    if (variables.find(atom.value) != variables.end())
    {
        return variables[atom.value];
    }
    else
    {
        std::size_t pos;
        try
        {
            std::stoi(atom.value, &pos);
            if (pos == atom.value.size())
                return std::stoi(atom.value);
            throw std::invalid_argument("");
        }
        catch (const std::invalid_argument &)
        {
            try
            {
                return std::stod(atom.value);
            }
            catch (const std::invalid_argument &)
            {
                throw std::runtime_error("Unsupported result type");
            }
        }
    }
}

int main()
{
    std::unordered_map<std::string, std::variant<int, double>> variables;
    while (true)
    {
        std::cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit")
            break;
        std::unordered_map<std::string, std::variant<int, double>> dummy;
        Expression expr = from_string(input, dummy);
        if (expr.is_assign())
        {
            auto lhs = expr.get_assign_lhs();
            expr = from_string(input, variables);
            auto rhs = expr.get_assign_rhs();
            variables[lhs] = evaluate(rhs, variables);
        }
        else
        {
            expr = from_string(input, variables);
            auto result = evaluate(expr, variables);
            std::visit([&](auto result)
                       { std::cout << result << std::endl; }, result);
        }
    }

    return 0;
}
