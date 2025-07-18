#pragma once

#include <stdexcept>
#include <string>
#include <cassert>
#include <utility>
#include <variant>

struct Token
{
    enum class Type
    {
        Atom,
        Op,
    };

    Type type;
    std::string value;

    Token() = delete;
    Token(Type type, std::string value) : type(type), value(std::move(value)) {}
};

inline std::pair<std::monostate, int> prefixBindingPower(const std::string &op)
{
    assert(op.size() == 1);
    switch (op[0])
    {
    case '+':
    case '-':
        // can only bind to the right
        return {std::monostate{}, 7};
    default:
        throw std::runtime_error("Unknown operator: " + op);
    }
}

inline std::pair<int, int> infixBindingPower(const std::string &op)
{
    assert(op.size() == 1);
    switch (op[0])
    {
    case '+':
    case '-':
        return {1, 2};
    case '*':
    case '/':
        return {3, 4};
    case '^':
        return {5, 6};
    case '|':
        return {9, 10};
    case '&':
        return {11, 12};
    default:
        throw std::runtime_error("Unknown operator: " + op);
    }
}
