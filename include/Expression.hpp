#pragma once

#include <queue>
#include <string>
#include <variant>
#include <vector>
#include <memory>

struct Expression
{
    struct Atom
    {
        std::string value;
    };

    struct Operation
    {
        std::string op;
        std::vector<std::shared_ptr<Expression>> operands;
    };

    std::variant<Atom, Operation> value{};
    std::shared_ptr<Expression> parent = nullptr;

    Expression() = default;
    Expression(Atom atom);
    Expression(Operation operation);
    bool isAtom() const { return std::holds_alternative<Atom>(value); }
    bool isOperation() const { return std::holds_alternative<Operation>(value); }
    std::shared_ptr<Expression> &getParent() { return parent; }

    std::string to_string() const;
};

void populateParentPointers(Expression &expr, std::shared_ptr<Expression> parent = nullptr);