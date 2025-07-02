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
    Expression parseExpression(float min_binding_power = -std::numeric_limits<float>::infinity());
};

Expression from_string(std::string_view input);