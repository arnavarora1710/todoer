#pragma once

#include <string>
#include <variant>
#include <vector>

struct Expression
{
    struct Atom
    {
        char value;
    };

    struct Operation
    {
        char op;
        std::vector<Expression> operands;
    };

    std::variant<Atom, Operation> value;

    Expression() = default;
    Expression(Atom atom);
    Expression(Operation operation);

    std::string to_string() const;
};
