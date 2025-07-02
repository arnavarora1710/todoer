#pragma once

#include <stdexcept>
#include <variant>
#include <utility>
#include <string>

struct Token
{
    enum class Type
    {
        Atom,
        Op,
    };

    Type type;
    char value;

    Token() = delete;
    Token(Type type, char value) : type(type), value(value) {}
};

inline std::pair<std::monostate, int> prefixBindingPower(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        // can only bind to the right
        return {std::monostate{}, 9};
    default:
        throw std::runtime_error("Unknown operator: " + std::string(1, op));
    }
}

inline std::pair<int, int> infixBindingPower(char op)
{
    switch (op)
    {
    case '=':
        return {1, 2};
    case '+':
    case '-':
        return {3, 4};
    case '*':
    case '/':
        return {5, 6};
    case '^':
        return {7, 8};
    default:
        throw std::runtime_error("Unknown operator: " + std::string(1, op));
    }
}
