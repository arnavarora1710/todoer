#include "Expression.hpp"

Expression::Expression(Atom atom) : value(atom) {}
Expression::Expression(Operation operation) : value(operation) {}

std::string Expression::to_string() const
{
    std::string result;
    if (std::holds_alternative<Atom>(value))
    {
        result.push_back(std::get<Atom>(value).value);
    }
    else
    {
        const auto &op = std::get<Operation>(value);
        result.push_back('(');
        result.push_back(op.op);
        for (const auto &arg : op.operands)
        {
            result.push_back(' ');
            result += arg.to_string();
        }
        result.push_back(')');
    }
    return result;
}
