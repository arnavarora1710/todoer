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

void populateParentPointers(Expression &expr, Expression *parent)
{
    expr.parent = parent;
    if (expr.isOperation())
    {
        for (auto &operand : std::get<Expression::Operation>(expr.value).operands)
        {
            populateParentPointers(operand, &expr);
        }
    }
}