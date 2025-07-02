#pragma once

#include <stdexcept>
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

inline std::pair<float, float> bindingPower(char op)
{
    switch (op)
    {
    case '=':
        return {0.0f, 0.1f};
    case '+':
    case '-':
        return {1.0f, 1.1f};
    case '*':
    case '/':
        return {2.0f, 2.1f};
    case '^':
        return {3.0f, 3.1f};
    default:
        throw std::runtime_error("Unknown operator: " + std::string(1, op));
    }
}