#pragma once

#include <queue>
#include <string>
#include <variant>
#include <vector>

struct Expression
{
    struct Atom
    {
        std::string value;
    };

    struct Operation
    {
        std::string op;
        std::vector<Expression> operands;
    };

    std::variant<Atom, Operation> value{};
    Expression *parent = nullptr;

    Expression() = default;
    Expression(Atom atom);
    Expression(Operation operation);
    bool isAtom() const { return std::holds_alternative<Atom>(value); }
    bool isOperation() const { return std::holds_alternative<Operation>(value); }
    Expression *getParent() const { return parent; }

    std::string to_string() const;
};

void populateParentPointers(Expression &expr, Expression *parent = nullptr);