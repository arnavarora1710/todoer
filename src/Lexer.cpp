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

std::shared_ptr<Expression> Lexer::parseExpression(VariableMap &variables, int min_binding_power)
{
    auto token = next();
    if (token->type == Token::Type::Atom ||
        (token->type == Token::Type::Op && (token->value == "(" || token->value == "+" || token->value == "-")))
    {
        std::shared_ptr<Expression> lhs;
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
                std::shared_ptr<Expression> rhs = parseExpression(variables, rbp);
                std::vector<std::shared_ptr<Expression>> operands;
                operands.push_back(rhs);
                lhs = std::make_shared<Expression>(Expression::Operation{token->value, operands});
            }
        }
        else
        {
            if (variables.find(token->value) != variables.end())
            {
                lhs = std::make_shared<Expression>(Expression::Atom{
                    std::visit([](auto v)
                               { return std::to_string(v); },
                               variables[token->value])});
            }
            else
            {
                lhs = std::make_shared<Expression>(Expression::Atom{token->value});
            }
        }

        // by this point, the lhs has been formed

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
                auto rhs = parseExpression(variables, rbp);
                std::vector<std::shared_ptr<Expression>> operands;
                operands.push_back(lhs);
                operands.push_back(rhs);
                lhs = std::make_shared<Expression>(Expression::Operation{next_token->value, operands});
            }
            else
            {
                throw std::runtime_error("Invalid token: " + next_token->value +
                                         "\nVariable names can only contain letters, digits, and underscores");
            }
        }
        return lhs;
    }
    else
    {
        throw std::runtime_error("Invalid token: " + token->value +
                                 "\nVariable names can only contain letters, digits, and underscores");
    }
}

std::shared_ptr<Expression> from_string(std::string_view input, VariableMap &variables)
{
    Lexer lexer(input);
    auto expr = lexer.parseExpression(variables, 0);
    populateParentPointers(*expr);
    return expr;
}
