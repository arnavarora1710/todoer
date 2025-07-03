#include "Expression.hpp"
#include <iostream>
#include <cassert>

Expression::Expression(Atom atom) : value(atom) {}
Expression::Expression(Operation operation) : value(operation) {}

std::string Expression::to_string() const
{
    std::string result;
    if (std::holds_alternative<Atom>(value))
    {
        result += (std::get<Atom>(value).value);
    }
    else
    {
        const auto &op = std::get<Operation>(value);
        result.push_back('(');
        result += op.op;
        for (const auto &arg : op.operands)
        {
            result.push_back(' ');
            result += arg.to_string();
        }
        result.push_back(')');
    }
    return std::move(result);
}

bool Expression::is_assign() const
{
    if (isOperation())
    {
        const auto &op = std::get<Operation>(value);
        if (op.op == "=")
        {
            const auto &atom = std::get<Atom>(op.operands[0].value);
            size_t pos;
            try
            {
                std::stoi(atom.value, &pos);
                if (pos == atom.value.length())
                {
                    throw std::invalid_argument("Number literals can't be assigned to!");
                }
            }
            catch (const std::invalid_argument &e)
            {
                // this is ok, it's a variable name
                return true;
            }
        }
    }
    return false;
}

std::string Expression::get_assign_lhs() const
{
    assert(is_assign());
    const auto &op = std::get<Operation>(value);
    if (op.operands[0].isAtom())
    {
        auto atom = std::get<Atom>(op.operands[0].value);
        return std::move(atom.value);
    }
    throw std::runtime_error("Expression is not an assignment");
}

Expression Expression::get_assign_rhs() const
{
    assert(is_assign());
    const auto &op = std::get<Operation>(value);
    return op.operands[1];
}