#pragma once

#include "Token.hpp"
#include "Expression.hpp"
#include <string_view>
#include <vector>
#include <optional>
#include <unordered_map>
#include <variant>
#include <memory>

using VariableMap = std::unordered_map<std::string, std::variant<int, double>>;

struct Lexer
{
    Lexer() = delete;
    explicit Lexer(std::string_view input);

    std::optional<Token> next() noexcept;
    std::optional<Token> peek() noexcept;

    std::vector<Token> tokens;
};

std::shared_ptr<Expression> from_string(std::string_view input, VariableMap &variables);
