#pragma once

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
    explicit Expression(Atom atom);
    explicit Expression(Operation operation);
    [[nodiscard]] bool isAtom() const { return std::holds_alternative<Atom>(value); }
    [[nodiscard]] bool isOperation() const { return std::holds_alternative<Operation>(value); }
    std::shared_ptr<Expression> &getParent() { return parent; }

    [[nodiscard]] std::string to_string() const;
};

void populateParentPointers(Expression &expr, const std::shared_ptr<Expression> &parent = nullptr);