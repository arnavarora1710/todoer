#pragma once

#include "Token.hpp"
#include "Expression.hpp"

#include <string_view>
#include <vector>
#include <optional>
#include <unordered_map>
#include <variant>

typedef std::unordered_map<std::string, std::variant<int, double>> VariableMap;

class Lexer
{
private:
    std::vector<Token> tokens;

public:
    Lexer() = delete;
    Lexer(std::string_view input);

    std::optional<Token> next() noexcept;
    std::optional<Token> peek() noexcept;
    std::shared_ptr<Expression> parseExpression(VariableMap &variables, int min_binding_power = 0);
};

std::shared_ptr<Expression> from_string(std::string_view input, VariableMap &variables);
