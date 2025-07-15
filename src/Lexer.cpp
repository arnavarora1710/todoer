#include "Lexer.hpp"
#include "Parser.hpp"
#include <cctype>
#include <algorithm>

Lexer::Lexer(std::string_view input)
{
    for (auto token_iter = input.begin(); token_iter != input.end();)
    {
        if (const auto token = *token_iter; token == ' ' || token == '\n')
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

std::shared_ptr<Expression> from_string(std::string_view input, VariableMap &variables)
{
    Lexer lexer(input);
    Parser parser(lexer, variables);
    auto expr = parser.parseExpression(0);
    populateParentPointers(*expr);
    return expr;
}
