#include "Lexer.hpp"
#include <cctype>
#include <stdexcept>
#include <algorithm>

Lexer::Lexer(std::string_view input)
{
    for (auto token_iter = input.begin(); token_iter != input.end();)
    {
        const auto token = *token_iter;
        if (token == ' ' || token == '\n')
        {
        }
        else if (std::isdigit(token) || std::isalpha(token))
        {
            std::string atom;
            while (token_iter != input.end() &&
                   (std::isdigit(*token_iter) || std::isalpha(*token_iter) ||
                    *token_iter == '.' || *token_iter == '_'))
            {
                atom += *token_iter;
                ++token_iter;
            }
            tokens.emplace_back(Token::Type::Atom, atom);
            continue;
        }
        else
            tokens.emplace_back(Token::Type::Op, std::string(1, token));
        ++token_iter;
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

Expression Lexer::parseExpression(VariableMap &variables, int min_binding_power)
{
    auto token = next();
    if (token->type == Token::Type::Atom ||
        (token->type == Token::Type::Op && (token->value == "(" || token->value == "+" || token->value == "-")))
    {
        Expression lhs;
        if (token->type == Token::Type::Op)
        {
            if (token->value == "(")
            {
                lhs = parseExpression(variables, 0);
                if (next()->value != ")")
                    throw std::runtime_error("Expected ) after (");
            }
            else
            {
                // has to be a prefix op (unary + or -)
                auto [_, rbp] = prefixBindingPower(token->value);
                auto rhs = parseExpression(variables, rbp);
                lhs = Expression(Expression::Operation{token->value, {rhs}});
            }
        }
        else
        {
            if (variables.find(token->value) != variables.end())
            {
                lhs = Expression(Expression::Atom{
                    std::visit([](auto v)
                               { return std::to_string(v); }, variables[token->value])});
            }
            else
            {
                lhs = Expression(Expression::Atom{token->value});
            }
        }
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
                if (next_token->value == ")")
                    break;

                auto [lbp, rbp] = infixBindingPower(next_token->value);
                if (lbp < min_binding_power)
                    break; // the left op is of higher priority, so stop tree construction here
                else
                    next(); // consume op
                Expression rhs = parseExpression(variables, rbp);
                lhs = Expression(Expression::Operation{next_token->value, {lhs, rhs}});
            }
            else
            {
                throw std::runtime_error("Unexpected Atom: " + next_token->value);
            }
        }
        return lhs;
    }
    else
    {
        throw std::runtime_error("Unexpected token while parsing expression");
    }
}

Expression from_string(std::string_view input, VariableMap &variables)
{
    Lexer lexer(input);
    return lexer.parseExpression(variables, 0);
}
