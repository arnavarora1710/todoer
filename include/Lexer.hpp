#pragma once

#include "Token.hpp"
#include "Expression.hpp"

#include <string_view>
#include <vector>
#include <optional>
#include <unordered_map>
#include <variant>

class Lexer
{
private:
    std::vector<Token> tokens;

public:
    Lexer() = delete;
    Lexer(std::string_view input);

    std::optional<Token> next() noexcept;
    std::optional<Token> peek() noexcept;
    Expression parseExpression(std::unordered_map<std::string, std::variant<int, double>> &variables, int min_binding_power = 0);
};

Expression from_string(std::string_view input, std::unordered_map<std::string, std::variant<int, double>> &variables);
