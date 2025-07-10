#include "Parser.hpp"

std::shared_ptr<Expression> Parser::parseExpression(int min_binding_power)
{
    auto token = lexer.next();
    if (token->type == Token::Type::Atom ||
        (token->type == Token::Type::Op && (token->value == "(" || token->value == "+" || token->value == "-")))
    {
        std::shared_ptr<Expression> lhs;
        if (token->type == Token::Type::Op)
        {
            if (token->value == "(")
            {
                lhs = parseExpression(0);
                if (lexer.next()->value != ")")
                    throw std::runtime_error("Expected ) after (");
            }
            else
            {
                // has to be a prefix op (unary + or -)
                auto [_, rbp] = prefixBindingPower(token->value);
                std::shared_ptr<Expression> rhs = parseExpression(rbp);
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
            auto next_token = lexer.peek();
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
                    lexer.next(); // consume op
                auto rhs = parseExpression(rbp);
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