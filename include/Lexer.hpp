#pragma once

#include "Token.hpp"
#include "Expression.hpp"

#include <string_view>
#include <vector>
#include <optional>

class Lexer
{
private:
    std::vector<Token> tokens;

public:
    Lexer() = delete;
    Lexer(std::string_view input);

    std::optional<Token> next() noexcept;
    std::optional<Token> peek() noexcept;
    Expression parseExpression(int min_binding_power = 0);
};

Expression from_string(std::string_view input);
