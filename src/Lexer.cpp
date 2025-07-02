#include "Lexer.hpp"

#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <limits>

Lexer::Lexer(std::string_view input)
{
    for (const auto &token : input)
    {
        if (token == ' ' || token == '\n')
            continue;
        else if (std::isdigit(token) || std::isalpha(token))
            tokens.emplace_back(Token::Type::Atom, token);
        else
            tokens.emplace_back(Token::Type::Op, token);
    }
    std::reverse(tokens.begin(), tokens.end());
}

std::optional<Token> Lexer::next() noexcept
{
    if (tokens.empty())
        return std::nullopt;

    Token token = tokens.back();
    tokens.pop_back();
    return token;
}

std::optional<Token> Lexer::peek() noexcept
{
    if (tokens.empty())
        return std::nullopt;
    return tokens.back();
}

Expression Lexer::parseExpression(float min_binding_power)
{
    auto token = next();
    if (token->type == Token::Type::Atom)
    {
        Expression lhs(Expression::Atom{token->value});
        while (true)
        {
            auto next_token = peek();
            // if no more tokens, return the atom
            if (!next_token)
            {
                return lhs;
            }
            else if (next_token->type == Token::Type::Op)
            {
                auto [lbp, rbp] = bindingPower(next_token->value);
                if (lbp < min_binding_power)
                    break; // the left op is of higher priority, so stop tree construction here
                else
                    next(); // consume op
                Expression rhs = parseExpression(rbp);
                lhs = Expression(Expression::Operation{next_token->value, {lhs, rhs}});
            }
            else
            {
                throw std::runtime_error("Unexpected Atom: " + std::string(1, next_token->value));
            }
        }
        return lhs;
    }
    else
    {
        throw std::runtime_error("Unexpected token while parsing expression");
    }
}

Expression from_string(std::string_view input)
{
    Lexer lexer(input);
    return lexer.parseExpression();
}